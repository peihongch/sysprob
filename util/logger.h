#ifndef LOGGER_H
#define LOGGER_H

#define LOG_INFO(msg, ...) \
    log_info("%s:%d:%s: " msg, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) \
    log_warn("%s:%d:%s: " msg, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) \
    log_error("%s:%d:%s: " msg, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

void log_info(const char *format, ...);
void log_warn(const char *format, ...);
void log_error(const char *format, ...);

#endif /* LOGGER_H */
