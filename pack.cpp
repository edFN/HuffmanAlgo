#define _CRT_SECURE_NO_WARNINGS


#include <string.h>

#include "pack.h"
#include "logger.h"
#include "encoding_tree.h"
#include <cassert>


static FILE* streamRead = NULL;
static FILE* streamWrite = NULL;
static HEADER_HUFFMAN huffman_header = NULL;
static int offset = 0;

#define CHUNK_SIZE 4096


#define READ_CHUNK(BUFFER,LEN) ((LEN) = fread((BUFFER),sizeof(unsigned char),CHUNK_SIZE,streamRead))
#define WRITE_CHUNK(BUFFER,LEN) fwrite((BUFFER),sizeof(unsigned char),(LEN),streamWrite)
#define WRITE_BYTE(VARIBLE) fwrite(&(VARIBLE),1,1,streamWrite)


//ÏÐÀÂÈÒÜ ÂÂÎÄ ÂÕÎÄÍÎÃÎ ÔÀÉËÀ

int init_module(const char* fileName, const char* fileOut) {

	fopen_s(&streamRead, fileName, "rb");

	if (streamRead == NULL) {
		error_pack = E_PACK_LOAD_FILE;
		return 0;
	}

	fopen_s(&streamWrite, fileOut, "wb");

	if (streamWrite == NULL) {
		error_pack = E_PACK_LOAD_FILE;
		fclose(streamRead);
		return 0;
	}

	huffman_header = (HEADER_HUFFMAN)malloc(sizeof(headerhuffman));

	if (huffman_header == NULL) {
		error_pack = E_PACK_BAD_ALLOC;

		fclose(streamRead);
		fclose(streamWrite);

		return 0;
	}

	return 1;

}

char* get_filename_ext(const char* filename) {
	
	char* extencion = (char*)malloc(20);

	if (extencion == NULL) {
		error_pack = E_PACK_BAD_ALLOC;
		return NULL;
	}

	const char* dot = strrchr(filename, '.');

	if (!dot || dot == filename) {
		free(extencion);
		return NULL;
	}

	memcpy(extencion, dot + 1, strlen(dot + 1)+1);

	return extencion;
}


void printNodes(tree_node* tree, vtype_tree_t value) {
	if (tree != NULL) {
		if (value == STRING_ELEM) {
			LOG_NOTE("%c - %s", tree->data.key.decimal, tree->data.value.string);
		}
		else {
			LOG_NOTE("%c - %d", tree->data.key.decimal, tree->data.value.decimal);
		}

		printNodes(tree->left,value);
		printNodes(tree->right,value);

	}
}


Tree* checkText()
{
	unsigned int nLen;
	unsigned char chunk[CHUNK_SIZE];
	
	Tree* counted_bytes = NULL;
	
	counted_bytes = new_tree(BYTE_ELEM, DECIMAL_ELEM);

	if (counted_bytes == NULL) {
		error_pack = E_PACK_BAD_ALLOC;
		return NULL;
	}

	while (READ_CHUNK(chunk, nLen) > 0) {
		
		for (unsigned i = 0; i < nLen; i++) {

			unsigned char key = chunk[i];

			tree_node* node = get_node(counted_bytes, decimal(key));

			if (node == NULL) {
				push_tree(counted_bytes, decimal(key), decimal(1));
			}
			else {
				node->data.value.decimal++;
			}
		}
	}


	fseek(streamRead, 0, SEEK_SET);

	return counted_bytes;
}

static int init_header(const char* in_file_name) {

	Tree* huffman_tree=NULL, 
		* check_alphabet = NULL;

	int nodes = 0;

	check_alphabet = checkText();

	if (check_alphabet == NULL)
		return 0;

	huffman_tree = build_encoding_tree(check_alphabet);

	
	nodes = len(huffman_tree);

	if (nodes == 0) {
		error_pack = E_PACK_EMPTY;
		return 0;
	}

	char* type = get_filename_ext(in_file_name);

	if (type == NULL) {
		error_pack = E_PACK_NOT_SPECIFIED_TYPE;
		return 0;
	}

	huffman_header->source_type = type;
	huffman_header->huffman_nodes = nodes;
	huffman_header->huffman_tree = huffman_tree;
	huffman_header->last_byte_used = 0;


	return 1;

}


//temp solution ... find better

static void writeNode(tree_node* node) {
	fwrite(&node->data.key.decimal,1,1,streamWrite);
	fwrite(&node->data.value.decimal, 4, 1, streamWrite);

	unsigned char isLeaf = childrens(node);
	fwrite(&isLeaf, 1, 1, streamWrite);
}

static void writeHeader() {

	fwrite(huffman_header->source_type, sizeof(unsigned char),
		strlen(huffman_header->source_type) + 1, streamWrite);

	fwrite(&huffman_header->huffman_nodes, 2, 1, streamWrite);

	//Ïðîïóñêàåì îäíó ïåðåìåííóþ
	offset = ftell(streamWrite);
	fseek(streamWrite, 1, SEEK_CUR);

	traverseTree(huffman_header->huffman_tree, writeNode);

}

static void writeContent() {
	
	int nLen;
	unsigned char chunk[CHUNK_SIZE];
	
	int bytes = 0;//updated
	unsigned char recv[CHUNK_SIZE*8];


	int result_byte = 0;
	int shift_bit = 7;


	Tree* key_codes = make_codes(huffman_header->huffman_tree);

	while (READ_CHUNK(chunk, nLen) > 0) {

		for (int i = 0; i < nLen; i++) {

			unsigned char currentByte = chunk[i];

			tree_node* node = get_node(key_codes, decimal(currentByte));

			char* code = node->data.value.string;

			while (*code) {

				result_byte |= (*code - '0') << shift_bit;
				shift_bit--;

				if (shift_bit < 0) {
					shift_bit = 7;
					recv[bytes++] = result_byte;
					result_byte = 0;
				}
				*code++;
			}
		};
		WRITE_CHUNK(recv, bytes);

		bytes = 0;
	}
	if (shift_bit != 7) {
		huffman_header->last_byte_used = 7 - shift_bit;
		WRITE_BYTE(result_byte);
	}
	else {
		huffman_header->last_byte_used = 8;
	}

	fseek(streamWrite, offset, SEEK_SET);
	WRITE_BYTE(huffman_header->last_byte_used);

	free_tree(key_codes);

}

static void write() {

	writeHeader();

	writeContent();

}

static void clear_module() {
	
	if(streamRead)
		fclose(streamRead);
	
	if(streamWrite)
		fclose(streamWrite);
	
	if (huffman_header) {
		if (huffman_header->huffman_tree)
			free_tree(huffman_header->huffman_tree);

		free(huffman_header);
	}
}


static const char* GetErrorString(error_pack_t errorCode) {

	switch (errorCode) {
		case E_PACK_BAD_ALLOC:
			return "Memory was not allocated\n";
		case E_PACK_EMPTY:
			return "File doesn't have content\n";
		case E_PACK_LOAD_FILE:
			return "Cant open file\n";
		case E_PACK_NOT_SPECIFIED_TYPE:
			return "Cant extract type of file\n";
		case E_PACK_NULL_REFERENCE:
			return "NULL reference\n";
		default:
			return "unknown error\n";
	}

}

void pack(const char* filename, const char* fileOut) {

	if (!init_module(filename, fileOut)) {
		fprintf(stderr, "Error: %s", GetErrorString(error_pack));
		return;
	}

	if (!init_header(filename)) {
		fprintf(stderr, "Error: %s", GetErrorString(error_pack));
		goto exit;
	}

	write();

exit:
	clear_module();
}


