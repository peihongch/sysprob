#include <stdio.h>
#include <string.h>

#include "cmd/cpu_cmd.h"
#include "cmd/mem_cmd.h"
#include "cmd/disk_cmd.h"
#include "cmd/net_cmd.h"
#include "plugin/plugin.h"
#include "util/logger.h"

/**
 * Logger configuration file:
 *
 *  log.level = INFO
 *  log.file = /var/log/sysprob.log
 */
#define LOGGER_CONFIG_FILE "sysprob.logcfg"

static int setup_logger() {
    log_level_t level;
    char *log_file;

    // read logger configuration from file
    FILE *file = fopen(LOGGER_CONFIG_FILE, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "log.level =", 11) == 0) {
                char *level_str = line + 11;

                while (*level_str == ' ')
                    level_str++;

                if (strncmp(level_str, "INFO", 4) == 0) {
                    level = LOG_LEVEL_INFO;
                } else if (strncmp(level_str, "WARN", 4) == 0) {
                    level = LOG_LEVEL_WARN;
                } else if (strncmp(level_str, "ERROR", 5) == 0) {
                    level = LOG_LEVEL_ERROR;
                } else {
                    level = LOG_LEVEL_INFO; // default
                }
            } else if (strncmp(line, "log.file =", 10) == 0) {
                log_file = line + 10;

                while (*log_file == ' ')
                    log_file++;

                char *newline = strchr(log_file, '\n');
                if (newline)
                    *newline = '\0';
            }
        }
        fclose(file);
    } else {
        // default configuration
        level = LOG_LEVEL_INFO;
        log_file = NULL;
    }

    if (logger_init(level, log_file) != 0) {
        fprintf(stderr, "Failed to initialize logger\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: sysprob cpu [interval]\n");
        return 0;
    }

    if (setup_logger() != 0) {
        printf("Failed to set up logger\n");
        return -1;
    }

    if (load_plugins() != 0) {
        LOG_ERROR("Failed to load plugins");
        return -1;
    }

    if (strcmp(argv[1], "cpu") == 0) {
        return run_cpu_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "mem") == 0) {
        return run_mem_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "disk") == 0) {
        return run_disk_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "net") == 0) {
        return run_net_cmd(argc - 1, argv + 1);
    } else {
        if (run_plugin(argv[1], argc - 1, argv + 1) != 0) {
            LOG_ERROR("Unknown command or plugin: %s", argv[1]);
            printf("Unknown command or plugin: %s\n", argv[1]);
            return -1;
        }
    }

    return 0;
}