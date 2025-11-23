#include <stdio.h>
#include <string.h>

#include "config.h"
#include "../util/logger.h"

#define CONFIG_FILE_PATH "/etc/sysprob.cfg"

/**
 * Logger configuration file:
 *
 *  log.level = INFO
 *  log.file = /var/log/sysprob.log
 */
#define LOGGER_CONFIG_FILE "sysprob.logcfg"

static int setup_logger(void) {
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

int load_config(config_t *config) {
    if (setup_logger() != 0) {
        fprintf(stderr, "Failed to set up logger\n");
        return -1;
    }

    FILE *file = fopen(CONFIG_FILE_PATH, "r");
    if (!file) {
        LOG_WARN("Unable to open config file: %s, use default config", CONFIG_FILE_PATH);
        return 0; // use default config
    }

    // read key-value parameters
    char line[256];
    char output_format[16] = "text";
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "interval = %d", &config->interval) == 1)
            continue;
        if (sscanf(line, "output_format = %15s", output_format) == 1) {
            config->output_format = strdup(output_format);
            continue;
        }
        if (sscanf(line, "cpu_probe = %d", (int *)&config->cpu_probe_enabled) == 1)
            continue;
        if (sscanf(line, "mem_probe = %d", (int *)&config->mem_probe_enabled) == 1)
            continue;
        if (sscanf(line, "disk_probe = %d", (int *)&config->disk_probe_enabled) == 1)
            continue;
        if (sscanf(line, "net_probe = %d", (int *)&config->net_probe_enabled) == 1)
            continue;
        if (sscanf(line, "plugin = %d", (int *)&config->plugin_enabled) == 1)
            continue;
    }
    fclose(file);

    return 0;
}
