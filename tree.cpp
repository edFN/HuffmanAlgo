
#include <string.h>

#include "tree.h"


Tree* new_tree(vtype_tree_t key, vtype_tree_t value) {
	switch (key) {
	case DECIMAL_ELEM:
	case STRING_ELEM:
	case BYTE_ELEM:
		break;

	default:
		fprintf(stderr, "%s\n", "key type nod supported");
		return NULL;
	}

	switch (value) {
	case DECIMAL_ELEM:
	case STRING_ELEM:
	case REAL_ELEM:
		break;
	default:
		fprintf(stderr, "%s\n", "value type doesn't supported");
		return NULL;
	}

	Tree* tree = (Tree*)malloc(sizeof(Tree));

	if (!tree) return NULL;

	tree->root = NULL;
	tree->type.key = key;
	tree->type.value = value;
	
	return tree;
}

void push_tree(Tree* tree, void* key, void* value)
{
	if (tree->root == NULL) {
		
		tree->root = new_node(tree->type.key, tree->type.value, key, value);
		return;
	}

	push_node_tree(tree->root, tree->type.key, tree->type.value, key, value);
}




static tree_node* find_node(tree_node* node, vtype_tree_t keyType, void* key) {

	if (node == NULL) return NULL;

	

	switch (keyType) {

	case DECIMAL_ELEM:
	case BYTE_ELEM:
		if (node->data.key.decimal == (unsigned)key) {
			return node;
		}

		if ((unsigned)key > node->data.key.decimal) {
			return find_node(node->right, keyType, key);
		}
		else {
			return find_node(node->left, keyType, key);
		}
		break;
	case STRING_ELEM:
		int compare = strcmp((char*)key, node->data.key.string);

		if (compare == 0) return node;

		if (compare > 1) return find_node(node->right, keyType, key);

		else {
			return find_node(node->left, keyType, key);
		}
		break;
	}


}

extern tree_node* get_node(Tree* tree, void* key)
{
	if (tree == NULL || tree->root == NULL)
		return NULL;

	return find_node(tree->root, tree->type.key, key);
	
}


void free_traverse(tree_node* node) {
	if (node) {
		free_traverse(node->left);
		free_traverse(node->right);
		free(node);
	}
}

void free_tree(Tree* tree)
{
	if (tree) {
		free_traverse(tree->root);
		free(tree);
	}
	
}

void* decimal(unsigned x)
{
	return (void*)x;
}

void* _string(char* str)
{
	return (void*)str;
}

void* real(double x)
{
	double* f = (double*)malloc(sizeof(double));
	*f = x;
	return (void*)f;
}

void _len_nodes(tree_node* node, int* result) {
	if (node) {
		*result = *result + 1;
		_len_nodes(node->left, result);
		_len_nodes(node->right, result);
	}
}

int len(Tree* tree)
{
	if (tree && tree->root) {
		int result = 0;
		_len_nodes(tree->root, &result);
		return result;
	}

	return 0;
}

int childrens(tree_node* node)
{
	if (node == NULL)
		return 0;

	if (node->left || node->right) 
		return 1;
	
	return 0;
	
}

tree_node* max(tree_node* tree)
{
	while (tree && tree->right) tree = tree->right;

	return tree;
}


//fix for all types
static void findMinValue(tree_node* tree, tree_node** min) {
	
	if (tree) {
		if (*min == NULL) 
			*min = tree;
		else {
			if (tree->data.value.decimal < (*min)->data.value.decimal) {
				*min = tree;
			}
		}
		findMinValue(tree->left, min);
		findMinValue(tree->right, min);

	}

}

tree_node* minValue(tree_node* tree)
{
	tree_node* min = NULL;

	findMinValue(tree, &min);
	
	return min;
}
tree_node* min(tree_node* tree) {
	while (tree != NULL && tree->left != NULL) tree = tree->left;

	return tree;
}

static void _traverse_tree(tree_node* node, void(*ptrFunction)(tree_node*)) {
	if (node) {
		ptrFunction(node);
		_traverse_tree(node->left, ptrFunction);
		_traverse_tree(node->right, ptrFunction);
	}
}

void traverseTree(Tree* tree, void(*ptrFunction)(tree_node*))
{
	if (tree == NULL) return;

	_traverse_tree(tree->root, ptrFunction);
}





static void push_node_tree(tree_node* node, vtype_tree_t keyType, vtype_tree_t valueType, 
	void* key, void* value) {

	switch (keyType) {
	case DECIMAL_ELEM:
	case BYTE_ELEM:
		
		if ((unsigned)key > node->data.key.decimal) {
			if (node->right == NULL) {
				
				
				node->right = new_node(keyType, valueType, key, value);
				node->right->parent = node;
				
			}
			else {
				push_node_tree(node->right, keyType, valueType, key, value);
			}
		}
		else if((unsigned)key < node->data.key.decimal) {
			if (node->left == NULL) {
				node->left = new_node(keyType, valueType, key, value);
				node->left->parent = node;
			}
			else {
				push_node_tree(node->left, keyType, valueType, key, value);
			}
		}
		break;
	case STRING_ELEM:
		if (strcmp((char*)key, node->data.key.string) > 0) {
			if (node->right == NULL) {
				node->right = new_node(keyType, valueType, key, value);
				node->right->parent = node;
			}
			else {
				push_node_tree(node->right, keyType, valueType, key, value);
			}
		}
		else {
			if (node->left == NULL) {
				node->left = new_node(keyType, valueType, key, value);
				node->left->parent = node;
			}
			else {
				push_node_tree(node->left, keyType, valueType, key, value);
			}

		}
		break;
	}


}

extern tree_node* new_node(vtype_tree_t keyType, vtype_tree_t valueType, void* key, void* value) {

	tree_node* node = (tree_node*)malloc(sizeof(tree_node));


	if (key) {

		switch (keyType) {
		case BYTE_ELEM:
		case DECIMAL_ELEM:
			node->data.key.decimal = (unsigned)key;
			break;
		case STRING_ELEM:
			node->data.key.string = (char*)key;
		}
	}
	else {
		node->data.key.decimal = 0;
	}
	if (value) {
		switch (valueType) {
		case DECIMAL_ELEM:
			node->data.value.decimal = (unsigned)value;
			break;
		case REAL_ELEM:
			node->data.value.real = *(double*)value;
			free((double*)value);
			break;
		case STRING_ELEM:
			node->data.value.string = (char*)value;
			break;
		}
	}
	

	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	return node;
}

tree_node* copy_node(tree_node* tree, vtype_tree_t keyType, vtype_tree_t valueType)
{
	if (!tree) return NULL;


	tree_node* n = new_node(keyType, valueType, NULL, NULL);


	switch (keyType) {
	case DECIMAL_ELEM:
	case BYTE_ELEM:
		n->data.key.decimal = tree->data.key.decimal;
		break;
	case STRING_ELEM:
		n->data.key.string = tree->data.key.string;
		break;
	}

	switch (valueType) {
	case DECIMAL_ELEM:
	case BYTE_ELEM:
		n->data.value.decimal = tree->data.value.decimal;
		break;
	case REAL_ELEM:
		n->data.value.real = tree->data.value.real;
		break;
	case STRING_ELEM:
		n->data.value.string = tree->data.value.string;
		break;
	}

	return n;


}

void remove_node(Tree* tree, tree_node* node, void(*pFreeFunc)(tree_node*)) {

	if (node == NULL) return;

	if (node->left == NULL && node->right == NULL) {
				
		if (node->parent) {

			if (node->parent->left == node) {
				node->parent->left = NULL;
			}
			else {
				node->parent->right = NULL;
			}

		}
		else {
			tree->root = NULL;
		}
				
		pFreeFunc(node);
	
	}
	else if (node->right != NULL && node->left != NULL) {
		tree_node* minNode = min(node->right);

		node->data = minNode->data;

		if (minNode == node->right) {
			minNode->parent->right = NULL;
		}
		else
			minNode->parent->left = NULL;

		pFreeFunc(minNode);
	}
	else{

		if (node->left) {

			if (node->parent == NULL) {
				tree_node* temp = node;
				tree->root = tree->root->left;
				tree->root->parent = NULL;
				pFreeFunc(temp);
			}
			else {
				node->left->parent = node->parent;
				node->parent->left = node->left;

				if (node->parent->left == node) {
					node->parent->left = node->left;
				}
				else {
					node->parent->right = node->left;
				}

				pFreeFunc(node);
			}
		}
		else {
			if (node->parent == NULL) {
				tree_node* temp = node;
				tree->root = tree->root->right;
				tree->root->parent = NULL;
				pFreeFunc(temp);

			}
			else {

				node->right->parent = node->parent;
				node->parent->right = node->right;

				if (node->parent->left == node) {
					node->parent->left = node->right;
				}
				else {
					node->parent->right = node->right;
				}

				pFreeFunc(node);
			}
		}
	}
	

}