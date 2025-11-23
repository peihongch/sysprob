#include <stdio.h>

#include "procfs_util.h"

int read_cpu_time(struct cpu_time *cpu_time) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Unable to open /proc/stat");
        return -1;
    }

    if (fscanf(file, "cpu %ld %ld %ld %ld",
               &cpu_time->user,
               &cpu_time->nice,
               &cpu_time->system,
               &cpu_time->idle) != 4) {
        perror("Failed to read /proc/stat");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int read_mem_info(struct mem_info *mem_info) {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) {
        perror("Unable to open /proc/meminfo");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %ld kB", &mem_info->total_kb) == 1)
            continue;
        if (sscanf(line, "MemFree: %ld kB", &mem_info->free_kb) == 1)
            continue;
    }

    fclose(file);
    return 0;
}

int read_net_dev_stats(struct net_dev_stats *stats) {
    FILE *file = fopen("/proc/net/dev", "r");
    if (!file) {
        perror("Unable to open /proc/net/dev");
        return -1;
    }

    char line[256];
    stats->rx_bytes = 0;
    stats->tx_bytes = 0;

    // Skip the first two header lines
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char iface[32];
        long rx_bytes, tx_bytes;
        if (sscanf(line, " %31[^:]: %ld %*s %*s %*s %*s %*s %*s %*s %ld",
                   iface, &rx_bytes, &tx_bytes) == 3) {
            stats->rx_bytes += rx_bytes;
            stats->tx_bytes += tx_bytes;
        }
    }

    fclose(file);
    return 0;
}
