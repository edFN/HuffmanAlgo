#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "unpack.h"
#include "logger.h"


static FILE* streamRead = NULL;
static FILE* streamWrite = NULL;
static HEADER_HUFFMAN huffman_header = NULL;


#define CHUNK_SIZE 4096

#define READ_ELEMENT(VAR) fread(&(VAR),sizeof((VAR)),1,streamRead)
#define READ_CHUNK(BUFFER,LEN) ((LEN) = fread((BUFFER),sizeof(unsigned char),CHUNK_SIZE,streamRead))
#define WRITE_BUFFER(BUFFER,LEN) fwrite(BUFFER,sizeof(unsigned char),(LEN),streamWrite)
//ÏÐÀÂÈÒÜ ÂÂÎÄ ÂÕÎÄÍÎÃÎ ÔÀÉËÀ



static int init_module(const char* fileIn) {

	fopen_s(&streamRead, fileIn, "rb");

	if (streamRead == NULL) {
		error_unpack = E_UNPACK_LOAD_FILE;
		return 0;
	}

	huffman_header = (HEADER_HUFFMAN)malloc(sizeof(headerhuffman));

	if (huffman_header == NULL) {
		
		error_unpack = E_UNPACK_BAD_ALLOC;

		fclose(streamRead);
		
		return 0;
		
	}

	return 1;
}

static char* read_type() {

	char* data = (char*)malloc(25);

	if (data == NULL) {
		error_unpack = E_UNPACK_BAD_ALLOC;
		return NULL;
	}

	char k,
		i = 0;

	while ((k = fgetc(streamRead))) 
		data[i++] = k;

	data[i] = '\0';

	return data;
}

static void _traverse_read_tree(tree_node** node) {

	unsigned char key;
	int value;
	unsigned char isLeaf;

	READ_ELEMENT(key);
	READ_ELEMENT(value);
	READ_ELEMENT(isLeaf);


	*node = new_node(BYTE_ELEM, DECIMAL_ELEM, decimal(key), decimal(value));

	//ÏËÞÑÛ ÌÎÃÓÒ ÁÛÒÜ ÑÎÄÅÐÆÈÌÛÌ ÔÀÉËÀ!!!!!
	if (isLeaf) {
		_traverse_read_tree(&((*node)->left));
		_traverse_read_tree(&((*node)->right));
	}
	
}

static Tree* read_tree() {
	
	Tree* tree = new_tree(BYTE_ELEM, DECIMAL_ELEM);

	if (tree == NULL) {
		error_unpack = E_UNPACK_BAD_ALLOC;
		return NULL;
	}

	_traverse_read_tree(&tree->root);

	return tree;

}


static int read_header() {
	
	Tree* tree = NULL;
	char* type = NULL;

	type = read_type();

	if (type == NULL) 
		return 0;
	
	huffman_header->source_type = type;

	READ_ELEMENT(huffman_header->huffman_nodes);

	READ_ELEMENT(huffman_header->last_byte_used);

	{
		tree = read_tree();

		if (tree == NULL)
			return 0;


		huffman_header->huffman_tree = tree;
	}
	
	return 1;
}

static char* make_file_name(const char* filename) {

	char* new_file_name = (char*)malloc(256);

	if (new_file_name == NULL) {
		error_unpack = E_UNPACK_BAD_ALLOC;
		return NULL;
	}

	strcpy(new_file_name, filename);
	strcat(new_file_name, ".");
	strcat(new_file_name, huffman_header->source_type);

	new_file_name[strlen(filename) + strlen(huffman_header->source_type) + 1] = '\0';

	return new_file_name;

}



//find better solution for last byte
static void writeContent() {

	int nLen;
	unsigned char* chunk = (unsigned char*)malloc(CHUNK_SIZE);

	int N=0;
	unsigned char *out_chunk = (unsigned char*) malloc(8*CHUNK_SIZE);

	tree_node* tree;

	if (!out_chunk || !chunk) {
		error_unpack = E_UNPACK_BAD_ALLOC;
		goto exit;
	}

	tree = huffman_header->huffman_tree->root;

	

	while (READ_CHUNK(chunk, nLen) > 0) {

		for (int i = 0; i < nLen; i++) {

			int off_bit = 7;

			if (i == nLen - 1 && feof(streamRead)) {
				off_bit = huffman_header->last_byte_used - 1;
			}

			unsigned char currentByte = chunk[i];

			for (int j = 0; j <= off_bit; j++) {

				if ((currentByte << j) & 0x80) {
					tree = tree->right;
				}
				else {
					tree = tree->left;
				}

				if (!childrens(tree)) {
					out_chunk[N++] = tree->data.key.decimal;
					tree = huffman_header->huffman_tree->root;
				}
			}

		}
		WRITE_BUFFER(out_chunk, N);
		N = 0;

	}
exit:
	if(out_chunk)
		free(out_chunk);
	if(chunk)
		free(chunk);

}


const char* GetErrorString(error_unpack_t errorCode) {

	switch (errorCode) {
	case E_UNPACK_BAD_ALLOC:
		return "Memory was not allocated\n";
	case E_UNPACK_EMPTY:
		return "File doesn't have content\n";
	case E_UNPACK_LOAD_FILE:
		return "Cant open file\n";
	case E_UNPACK_NULL_REFERENCE:
		return "NULL reference\n";
	default:
		return "unknown error\n";
	}

}

static void open_write_file(const char* outFile) {

	char* new_file = make_file_name(outFile);

	if (new_file == NULL) return;


	fopen_s(&streamWrite, new_file, "wb");

	if (streamWrite == NULL) {
		error_unpack = E_UNPACK_LOAD_FILE;
	}

	free(new_file);

}


static void clear_module() {

	if (streamRead)
		fclose(streamRead);

	if (streamWrite)
		fclose(streamWrite);

	if (huffman_header) {
		if (huffman_header->huffman_tree)
			free_tree(huffman_header->huffman_tree);

		free(huffman_header);
	}
}


void unpack(const char* inFile, const char* outFile) {

	if (!init_module(inFile)) {
		fprintf(stderr, "Error: %s", GetErrorString(error_unpack));
		goto exit;
	}

	if (!read_header()) {
		fprintf(stderr, "Error: %s", GetErrorString(error_unpack));
		goto exit;
	}

	open_write_file(outFile);

	if (error_unpack != E_UNPACK_OK) {
		fprintf(stderr, "Error: %s", GetErrorString(error_unpack));
		goto exit;
	}


	writeContent();


exit:
	clear_module();


}