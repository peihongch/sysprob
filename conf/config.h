#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    int interval;
    char *output_format;
    bool cpu_probe_enabled;
    bool mem_probe_enabled;
    bool disk_probe_enabled;
    bool net_probe_enabled;
    bool plugin_enabled;
} config_t;

int load_config(config_t *config);

#endif /* CONFIG_H */
