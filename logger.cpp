#include <cstdarg>
#include <malloc.h>
#include "logger.h"


static FILE* streamWriter = NULL;

void init_logger(const char* filename) {

	time_t clock;

	clock = time(NULL);
	
	char* log_file = (char*)malloc(120);
	
	snprintf(log_file, 119, "%s%d.log", filename, clock / 3600);

	fopen_s(&streamWriter, log_file, "w");

	free(log_file);




}

void write_logger(log_type_t loggerType, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);


	switch (loggerType) {
	case SUCCESS:
		fprintf(streamWriter,"[SUCESS]");
		break;
	case WARNING:
		fprintf(streamWriter, "[WARNING]");
		break;
	case ERROR:
		fprintf(streamWriter, "[ERROR]");
		break;
	default:break;
	}

	vfprintf(streamWriter,fmt, args);

	fprintf(streamWriter,"\n");

}

void close_logger()
{
	if (streamWriter != NULL) {
		fclose(streamWriter);
	}
}


