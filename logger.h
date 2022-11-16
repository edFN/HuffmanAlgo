#pragma once
#include <stdio.h>
#include <time.h>

typedef enum {
	SUCCESS,
	WARNING,
	ERROR,
	NONE
}log_type_t;

#define LOG_WARNING(fmt, ...) write_logger(WARNING,(fmt),##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) write_logger(ERROR,(fmt),##__VA_ARGS__)
#define LOG_SUCCESS(fmt, ...) write_logger(WARNING,(fmt),##__VA_ARGS__)
#define LOG_NOTE(fmt,...) write_logger(NONE,(fmt),##__VA_ARGS__)


void init_logger(const char* fileName);

void write_logger(log_type_t loggerType, const char* fmt, ... );

void close_logger();
