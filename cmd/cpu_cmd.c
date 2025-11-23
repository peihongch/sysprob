#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu_cmd.h"
#include "../core/probe.h"
#include "../core/cpu_probe.h"
#include "../util/logger.h"

int run_cpu_cmd(config_t *config, int argc, char *argv[]) {
    Probe *cpu_probe;
    ProbeOptions options = {
        .interval = config->interval,
        .report_format = REPORTER_FORMAT_TEXT,
    };
    cpu_usage_type_t type = CPU_USAGE_TOTAL;
    int i;

    if (config->output_format != NULL) {
        if (strcmp(config->output_format, "csv") == 0) {
            options.report_format = REPORTER_FORMAT_CSV;
        } else if (strcmp(config->output_format, "json") == 0) {
            options.report_format = REPORTER_FORMAT_JSON;
        }
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "user") == 0) {
            type = CPU_USAGE_USER;
        }
    }

    cpu_probe = get_cpu_probe();
    if (!cpu_probe) {
        LOG_ERROR("Failed to create CPU probe");
        return -1;
    }

    options.extra = (void *)type;
    run_probe(cpu_probe, &options);

    return 0;
}
