#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_cmd.h"

#include "../core/cpu_probe.h"
#include "../util/log_util.h"

static void run_cpu_usage_probe(int interval, cpu_usage_type_t type) {
    double usage;

    while (1) {
        usage = probe_cpu_usage(interval, type);
        switch (type) {
        case CPU_USAGE_TOTAL:
            printf("CPU: %.2f%%\n", usage);
            break;
        case CPU_USAGE_USER:
            printf("CPU User: %.2f%%\n", usage);
            break;
        }
    }
}

int run_cpu_cmd(int argc, char *argv[]) {
    cpu_usage_type_t type;
    int interval = 1;

    if (argc >= 2 && strcmp(argv[1], "user") == 0) {
        type = CPU_USAGE_USER;
    } else {
        type = CPU_USAGE_TOTAL;
    }

    if (argc >= 3) {
        interval = atoi(argv[2]);
        if (interval <= 0) {
            interval = 1;
            LOG_WARN("Interval must be a positive number, use default %ds.", interval);
        }
    }

    run_cpu_usage_probe(interval, type);

    return 0;
}
