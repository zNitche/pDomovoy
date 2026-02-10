#include "../../includes/debug_print.h"
#include <stdarg.h>
#include <stdio.h>

void debug_print(const char* format, ...) {
#ifndef DEBUG
    return;
#endif

    va_list args;
    va_start(args, format);

    printf(format, args);
    va_end(args);
}