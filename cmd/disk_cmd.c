#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disk_cmd.h"
#include "../core/probe.h"
#include "../core/disk_probe.h"
#include "../util/logger.h"

int run_disk_cmd(int argc, char *argv[]) {
    Probe *disk_probe;
    ProbeOptions options = {
        .interval = 1,
        .report_format = REPORTER_FORMAT_TEXT,
    };
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--output=csv") == 0) {
            options.report_format = REPORTER_FORMAT_CSV;
        } else if (strcmp(argv[i], "--output=json") == 0) {
            options.report_format = REPORTER_FORMAT_JSON;
        } else if (atoi(argv[i]) > 0) {
            options.interval = atoi(argv[i]);
        }
    }

    disk_probe = get_disk_probe();
    if (!disk_probe) {
        LOG_ERROR("Failed to create Disk probe");
        return -1;
    }

    options.extra = NULL;
    run_probe(disk_probe, &options);

    return 0;
}
