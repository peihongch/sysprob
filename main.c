#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd/cpu_cmd.h"
#include "cmd/mem_cmd.h"
#include "cmd/disk_cmd.h"
#include "cmd/net_cmd.h"
#include "cmd/summary_cmd.h"
#include "plugin/plugin.h"
#include "util/logger.h"
#include "conf/config.h"

int main(int argc, char *argv[]) {
    config_t config = {
        .interval = 1,
        .output_format = "text",
        .cpu_probe_enabled = 1,
        .mem_probe_enabled = 1,
        .disk_probe_enabled = 1,
        .net_probe_enabled = 1,
        .plugin_enabled = 1,
    };
    int i;

    if (argc < 2) {
        printf("Usage: sysprob cpu [interval]\n");
        return 0;
    }

    if (load_config(&config) != 0) {
        fprintf(stderr, "Failed to load configuration\n");
        return -1;
    }

    if (load_plugins() != 0) {
        LOG_ERROR("Failed to load plugins");
        return -1;
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--output=csv") == 0) {
            config.output_format = "csv";
        } else if (strcmp(argv[i], "--output=json") == 0) {
            config.output_format = "json";
        } else if (atoi(argv[i]) > 0) {
            config.interval = atoi(argv[i]);
        }
    }

    if (strcmp(argv[1], "summary") == 0) {
        return run_summary_cmd(&config, argc - 1, argv + 1);
    } else if (strcmp(argv[1], "cpu") == 0) {
        if (!config.cpu_probe_enabled) {
            LOG_ERROR("CPU probe is disabled in the configuration");
            printf("CPU probe is disabled in the configuration\n");
            return -1;
        }
        return run_cpu_cmd(&config, argc - 1, argv + 1);
    } else if (strcmp(argv[1], "mem") == 0) {
        if (!config.mem_probe_enabled) {
            LOG_ERROR("Memory probe is disabled in the configuration");
            printf("Memory probe is disabled in the configuration\n");
            return -1;
        }
        return run_mem_cmd(&config, argc - 1, argv + 1);
    } else if (strcmp(argv[1], "disk") == 0) {
        if (!config.disk_probe_enabled) {
            LOG_ERROR("Disk probe is disabled in the configuration");
            printf("Disk probe is disabled in the configuration\n");
            return -1;
        }
        return run_disk_cmd(&config, argc - 1, argv + 1);
    } else if (strcmp(argv[1], "net") == 0) {
        if (!config.net_probe_enabled) {
            LOG_ERROR("Network probe is disabled in the configuration");
            printf("Network probe is disabled in the configuration\n");
            return -1;
        }
        return run_net_cmd(&config, argc - 1, argv + 1);
    } else {
        if (!config.plugin_enabled) {
            LOG_ERROR("Plugin support is disabled in the configuration");
            printf("Plugin support is disabled in the configuration\n");
            return -1;
        }
        if (run_plugin(argv[1], &config, argc - 1, argv + 1) != 0) {
            LOG_ERROR("Unknown command or plugin: %s", argv[1]);
            printf("Unknown command or plugin: %s\n", argv[1]);
            return -1;
        }
    }

    return 0;
}