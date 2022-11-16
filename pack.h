#pragma once
#include "packed.h"

typedef enum pack_error {
	E_PACK_OK,
	E_PACK_LOAD_FILE,
	E_PACK_NULL_REFERENCE,
	E_PACK_BAD_ALLOC,
	E_PACK_NOT_SPECIFIED_TYPE,
	E_PACK_EMPTY
}error_pack_t;

static error_pack_t error_pack = E_PACK_OK;

typedef headerhuffman* HEADER_HUFFMAN;


void pack(const char* fileName, const char* fileOut);

static int init_header(const char* in_file_name);


//Make huffman Tree
static Tree* checkText();


static void write();

static void writeNode(tree_node*);
static void writeHeader();
static void writeContent();


static const char* GetErrorString(error_pack_t errorCode);

int init_module();

static void clear_module();

