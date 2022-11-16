#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "string.h"

static int allocated = 0;
static int deallocated = 0;

string init(size_t maxLength) {
	
	string str = (string)malloc(sizeof(string_data));

	if (str != NULL) {
		
		str->maxLength = maxLength;

		str->str = (char*)malloc(str->maxLength);

		str->N = 0;

		allocated += str->maxLength + sizeof(string_data);

		return str;
	}

	return str;
}

void write_string(string str, char* text) {

	size_t len_text = strlen(text);

	if (len_text >= str->maxLength)
		return;

	memcpy(str->str, text, len_text);

	str->N = len_text;

	str->str[str->N] = 0;

}

void string_to_char_array(string str, char* arr) {
	memcpy(arr, str->str, str->N);
	arr[str->N] = 0;
}

void copy(string dst, string src)
{
	if (dst->maxLength <= src->N)
		return;

	memcpy(dst->str, src->str, src->N);
	dst->N = src->N;
	dst->str[dst->N] = 0;
}

void concat(string dest, string src) {

	if (dest->maxLength <= dest->N + src->N) {
		return;
	}

	memcpy(dest->str + dest->N, src->str, src->N);
	dest->N += src->N;

	dest->str[dest->N] = 0;
}

void concat(string dest, const char* src) {
	
	size_t len_text = strlen(src);

	if (len_text + dest->N >= dest->maxLength) {
		return;
	}

	memcpy(dest->str + dest->N, src, len_text);
	dest->N += len_text;

	dest->str[dest->N] = 0;

}

string concat_build(string destination, const char* src)
{
	int newSize = destination->N + strlen(src) + 1;
	
	string newStr = init(newSize);

	copy(newStr, destination);


	concat(newStr, src);

	return newStr;

}


void set_char(string dest, int index, char c) {
	
	if (dest->N < index) {
		return;
	}

	dest->str[index] = c;
}

char* char_copy_of(string str){
	char* data = (char*)malloc(str->N + 1);
	memcpy(data, str->str, str->N + 1);
	return data;
}

void clear(string str) {
	if (str->str != NULL) {

		deallocated += str->maxLength;
		free(str->str);
	}
	free(str);
	deallocated += sizeof(string_data);
}

void _debugMemory() {

	printf("-----------STRING MODULE-------------\n");

	printf("Allocated: %d\n", allocated);
	printf("DeAllocated: %d\n", deallocated);

	printf("------------------------------------\n");
}