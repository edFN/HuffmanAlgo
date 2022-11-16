#pragma once

#include "tree.h"


typedef struct PackedFile {

	char* source_type;

	unsigned short huffman_nodes;
	unsigned char last_byte_used;
	
	Tree* huffman_tree;

}_packed_,headerhuffman;
