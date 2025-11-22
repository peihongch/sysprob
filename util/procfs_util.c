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
