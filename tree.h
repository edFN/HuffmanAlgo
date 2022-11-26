#pragma once
#include <malloc.h>
#include <stdio.h>


typedef enum {
	NONE_ELEM,
	DECIMAL_ELEM,
	REAL_ELEM,
	STRING_ELEM,
	BYTE_ELEM
}vtype_tree_t;


typedef union {
	unsigned decimal;
	double real;
	char* string;
}value_tree_t;

typedef struct tree_node {
	
	struct {
		value_tree_t key;
		value_tree_t value;
	}data;

	struct tree_node* left;
	struct tree_node* right;
	struct tree_node* parent;
}tree_node;


typedef struct Tree {
	struct {
		vtype_tree_t key;
		vtype_tree_t value;
	}type;

	struct tree_node* root;
}Tree;


extern Tree* new_tree(vtype_tree_t key, vtype_tree_t value);


extern void push_tree(Tree* tree, void* key, void* value);

extern void remove_node(Tree* node,tree_node* nodeDelete,
	void (*pFreeFunc)(tree_node*));

extern tree_node* new_node(vtype_tree_t keyType, vtype_tree_t valueType,
	void* key, void* value);


tree_node* copy_node(tree_node* tree, vtype_tree_t keyType, vtype_tree_t valueType);


extern tree_node* get_node(Tree* tree, void* key);


extern void free_tree(Tree* tree);

extern void* decimal(unsigned x);
extern void* _string(char* str);
extern void* real(double x);

extern int len(Tree* tree);


extern int childrens(tree_node* tree);

extern tree_node* maxValue(tree_node* tree);
extern tree_node* minValue(tree_node* tree);


extern void traverseTree(Tree* tree, void (*ptrFunction)(tree_node*));

static void push_node_tree(tree_node*, vtype_tree_t key, vtype_tree_t value, void* keyq, void* valueq);