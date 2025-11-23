#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_cmd.h"

#include "../core/probe.h"
#include "../core/cpu_probe.h"
#include "../util/log_util.h"

int run_cpu_cmd(int argc, char *argv[]) {
    Probe *cpu_probe;
    ProbeOptions options;
    int interval = 1;
    cpu_usage_type_t type;

    if (argc >= 2 && strcmp(argv[1], "user") == 0) {
        type = CPU_USAGE_USER;
        argc--;
        argv++;
    } else {
        type = CPU_USAGE_TOTAL;
    }

    if (argc >= 2) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            interval = 1;
            LOG_WARN("Interval must be a positive number, use default %ds.", interval);
        }
    }

    cpu_probe = get_cpu_probe();
    if (!cpu_probe) {
        LOG_ERROR("Failed to create CPU probe");
        return -1;
    }

    options.interval = interval;
    options.extra = (void *)type;
    run_probe(cpu_probe, &options);

    return 0;
}
