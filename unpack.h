#pragma once
#include "packed.h"

typedef enum unpack_error {
	E_UNPACK_OK,
	E_UNPACK_BAD_ALLOC,
	E_UNPACK_LOAD_FILE,
	E_UNPACK_NULL_REFERENCE,
	E_UNPACK_READ_FILE,
	E_UNPACK_EMPTY
}error_unpack_t;

typedef headerhuffman* HEADER_HUFFMAN;

static error_unpack_t error_unpack = E_UNPACK_OK;

static int init_module(const char* fileIn, const char* fileOut);
static void clear_module();

static int read_header();

static Tree* read_tree();
static void _traverse_read_tree(tree_node** node);

static const char* GetErrorString(error_unpack_t errorCode);

static void write();
static void writeContent();

extern void unpack(const char* inFile,const char* outFile);


