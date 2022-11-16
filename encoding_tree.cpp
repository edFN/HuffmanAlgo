
#include "encoding_tree.h"
#include "priority_queue.h"

#include "string.h"

#include <string>

//Пройтись по keys и добавить элементы праивльно!!
static void _fill_priority_queue(priority_queue* q, tree_node* root,
	vtype_tree_t key, vtype_tree_t value) {
	if (root) {

		tree_node* node = copy_node(root, key, value);
		pq_insert(q, node);

		_fill_priority_queue(q, root->left, key, value);
		_fill_priority_queue(q, root->right, key, value);

	}
}
static void fill_priority_queue(priority_queue* q, Tree* keys) {

	if (!keys || !q)
		return;

	_fill_priority_queue(q, keys->root, keys->type.key, 
		keys->type.value);

	//later delete
	free_tree(keys);

}



Tree* build_encoding_tree(Tree* keys) {
	
	priority_queue q;

	pq_init(&q, BYTE_ELEM, DECIMAL_ELEM);

	

	Tree* encoding_tree = new_tree(BYTE_ELEM, DECIMAL_ELEM);

	fill_priority_queue(&q, keys);



	while (q.n > 1) 
	{

		tree_node* curParent = new_node(BYTE_ELEM, DECIMAL_ELEM,NULL,NULL);


		tree_node* node1 = pq_extract_min(&q);

		tree_node* node2 = pq_extract_min(&q);
		
	
		curParent->left = node1;
		curParent->right = node2;


		curParent->left->parent = curParent;
		curParent->right->parent = curParent;

		
		int sum = curParent->left->data.value.decimal + curParent->right->data.value.decimal;

		curParent->data.value.decimal = sum;
		curParent->data.key.decimal = '+';
		
		pq_insert(&q, curParent);
		
		encoding_tree->root = curParent;
	}
	

	return encoding_tree;
}


void _get_key_codes(Tree* key_codes, tree_node* encoding_node, string code) {

	if (encoding_node == NULL)
		return;

	if (!childrens(encoding_node)) {
		push_tree(key_codes, decimal(encoding_node->data.key.decimal),
			string(char_copy_of(code)));
	}

	string plusZero = concat_build(code, "0");

	_get_key_codes(key_codes, encoding_node->left, plusZero);

	clear(plusZero);


	string plusOne = concat_build(code, "1");

	_get_key_codes(key_codes, encoding_node->right, plusOne);

	clear(plusOne);

}


void get_key_codes(Tree* key_codes, Tree* encoding_tree) {
	
	if (key_codes->type.key != BYTE_ELEM && key_codes->type.key != DECIMAL_ELEM) {
		return;
	}

	if (key_codes->type.value != STRING_ELEM)
		return;

	string code = init(120);

	_get_key_codes(key_codes, encoding_tree->root, code);

	clear(code);

}






Tree* make_codes(Tree* huffmanTree)
{
	Tree* result = new_tree(BYTE_ELEM, STRING_ELEM);

	get_key_codes(result, huffmanTree);

	return result;

}



