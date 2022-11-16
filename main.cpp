

#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "pack.h"
#include "unpack.h"
#include "logger.h"



enum Usage{
	U_PACK,
	U_DECODE,
	U_UNDEFINED
};

int get_flag(char* argv) {
	
	while (*argv == '-' || *argv == ' ') {
		*argv++;
	}
	
	if (*argv == 'p') return U_PACK;
	
	if (*argv == 'u') return U_DECODE;

	return U_UNDEFINED;

}

int main(int argc, char* argv[]) {
	
	setlocale(LC_ALL, "en");
	
	

	if (argc != 4) {
		printf("Используйте ./pack -[u/p] [file_in] [file_out]\n");
		printf("-u - распаковать файл\n");
		printf("-p - запаковать файл\n");
		return 1;
	}


	init_logger("unpack");

	char* in_path = argv[2];
	char* out_path = argv[3];

	int flag = get_flag(argv[1]);
	
	if (flag == U_UNDEFINED) {
		printf("Underfined command\n");
		goto exit;
	}

	if (flag == U_PACK) {
		pack(in_path, out_path);
	}
	else {
		unpack(in_path, out_path);
	}

exit:
	close_logger();


	return 0;

	

}

