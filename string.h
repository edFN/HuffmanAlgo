#pragma once


struct string_data
{
	char* str;

	size_t maxLength;
	size_t N;
};

typedef string_data* string;

string init(size_t maxLength);

//добавить в str содержимое из text
void write_string(string str, char* text);

//ѕреобразовать строку в массив из char
void string_to_char_array(string str, char* arr);

void copy(string dst, string src);


void concat(string destination, string source);
void concat(string destination, const char* src);

string concat_build(string destination, const char* src);


void clear(string str);

//¬ыставл€ет в букву c в позицию index строки destination
void set_char(string destination, int index, char c);

char* char_copy_of(string str);


void _debugMemory();







