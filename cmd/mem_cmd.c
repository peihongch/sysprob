#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem_cmd.h"
#include "../core/probe.h"
#include "../core/mem_probe.h"
#include "../util/logger.h"

int run_mem_cmd(config_t *config, int argc, char *argv[]) {
    Probe *mem_probe;
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

    mem_probe = get_mem_probe();
    if (!mem_probe) {
        LOG_ERROR("Failed to create Memory probe");
        return -1;
    }

    options.extra = NULL;
    run_probe(mem_probe, &options);

    return 0;
}
