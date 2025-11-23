#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stdout, "%s [INFO] ", __TIME__);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    va_end(args);
}

void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stdout, "%s [WARN] ", __TIME__);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    va_end(args);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s [ERROR] ", __TIME__);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}
