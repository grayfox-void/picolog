#include "picolog.h"
#include <stdarg.h>

#define MAX_BUFFER 1024

void _picolog_write(int level, int id, const char *fmt, ...)
{
	uint8_t _buff[MAX_BUFFER];
	va_list _args;
	va_start(_args, fmt);

	while (*fmt) {
		switch (*fmt++) {
		case 's':
			break;

		case 'd':
			break;

		case 'c':
			break;
		}
	}

	va_end(_args);
}
