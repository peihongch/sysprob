#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu_cmd.h"
#include "../core/probe.h"
#include "../core/cpu_probe.h"
#include "../util/logger.h"

int run_cpu_cmd(int argc, char *argv[]) {
    Probe *cpu_probe;
    ProbeOptions options = {
        .interval = 1,
        .report_format = REPORTER_FORMAT_TEXT,
    };
    cpu_usage_type_t type = CPU_USAGE_TOTAL;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--output=csv") == 0) {
            options.report_format = REPORTER_FORMAT_CSV;
        } else if (strcmp(argv[i], "user") == 0) {
            type = CPU_USAGE_USER;
        } else if (atoi(argv[i]) > 0) {
            options.interval = atoi(argv[i]);
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
