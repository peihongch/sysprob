#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net_cmd.h"
#include "../core/probe.h"
#include "../core/net_probe.h"
#include "../util/logger.h"

int run_net_cmd(config_t *config, int argc, char *argv[]) {
    Probe *net_probe;
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

    net_probe = get_net_probe();
    if (!net_probe) {
        LOG_ERROR("Failed to create Network probe");
        return -1;
    }

    options.extra = NULL;
    run_probe(net_probe, &options);

    return 0;
}
