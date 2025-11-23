#include <stdio.h>
#include <stdlib.h>

#include "disk_cmd.h"
#include "../core/probe.h"
#include "../core/disk_probe.h"
#include "../util/log_util.h"

int run_disk_cmd(int argc, char *argv[]) {
    Probe *disk_probe;
    ProbeOptions options;
    int interval = 1;

    if (argc >= 2) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            interval = 1;
            LOG_WARN("Interval must be a positive number, use default %ds.", interval);
        }
    }

    disk_probe = get_disk_probe();
    if (!disk_probe) {
        LOG_ERROR("Failed to create Disk probe");
        return -1;
    }

    options.interval = interval;
    options.extra = NULL;
    run_probe(disk_probe, &options);

    return 0;
}
