#include <stdio.h>
#include <stdlib.h>

#include "net_cmd.h"
#include "../core/probe.h"
#include "../core/net_probe.h"
#include "../util/logger.h"

int run_net_cmd(int argc, char *argv[]) {
    Probe *net_probe;
    ProbeOptions options;
    int interval = 1;

    if (argc >= 2) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            interval = 1;
            LOG_WARN("Interval must be a positive number, use default %ds.", interval);
        }
    }

    net_probe = get_net_probe();
    if (!net_probe) {
        LOG_ERROR("Failed to create Network probe");
        return -1;
    }

    options.interval = interval;
    options.extra = NULL;
    run_probe(net_probe, &options);

    return 0;
}
