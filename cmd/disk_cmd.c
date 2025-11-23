#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disk_cmd.h"
#include "../core/probe.h"
#include "../core/disk_probe.h"
#include "../util/logger.h"

int run_disk_cmd(config_t *config, int argc, char *argv[]) {
    Probe *disk_probe;
    ProbeOptions options = {
        .interval = config->interval,
        .report_format = REPORTER_FORMAT_TEXT,
    };

    if (config->output_format != NULL) {
        if (strcmp(config->output_format, "csv") == 0) {
            options.report_format = REPORTER_FORMAT_CSV;
        } else if (strcmp(config->output_format, "json") == 0) {
            options.report_format = REPORTER_FORMAT_JSON;
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
