#include <stdio.h>
#include <stdlib.h>

#include "mem_cmd.h"
#include "../core/probe.h"
#include "../core/mem_probe.h"
#include "../util/logger.h"

int run_mem_cmd(int argc, char *argv[]) {
    Probe *mem_probe;
    ProbeOptions options;
    int interval = 1;

    if (argc >= 2) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            interval = 1;
            LOG_WARN("Interval must be a positive number, use default %ds.", interval);
        }
    }

    mem_probe = get_mem_probe();
    if (!mem_probe) {
        LOG_ERROR("Failed to create Memory probe");
        return -1;
    }

    options.interval = interval;
    options.extra = NULL;
    run_probe(mem_probe, &options);

    return 0;
}
