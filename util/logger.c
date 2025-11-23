#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

typedef struct {
    log_level_t level;
    FILE *log_file;
} logger_t;

static logger_t global_logger = {
    .level = LOG_LEVEL_INFO,
    .log_file = NULL
};

int logger_init(log_level_t level, const char *log_file) {
    global_logger.level = level;
    if (log_file) {
        global_logger.log_file = fopen(log_file, "a");
        if (!global_logger.log_file) {
            perror("Failed to open log file");
            return -1;
        }
    } else {
        global_logger.log_file = stdout;
    }
    return 0;
}

static void log_message(log_level_t level, const char *level_str,
                        const char *format, va_list args) {
    if (level < global_logger.level) {
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    if (global_logger.log_file) {
        fprintf(global_logger.log_file, "%s [%s] ", time_buf, level_str);
        vfprintf(global_logger.log_file, format, args);
        fprintf(global_logger.log_file, "\n");
        fflush(global_logger.log_file);
    }
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_INFO, "INFO", format, args);
    va_end(args);
}

void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_WARN, "WARN", format, args);
    va_end(args);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_ERROR, "ERROR", format, args);
    va_end(args);
}
