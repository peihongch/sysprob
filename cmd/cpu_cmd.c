#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_cmd.h"
#include "../core/cpu_probe.h"

int run_cpu_cmd(int argc, char *argv[]) {
    int interval = 1;

    if (argc >= 2) {
        interval = atoi(argv[1]);
        if (interval <= 0) {
            interval = 1;
            fprintf(stderr, "Interval must be a positive number, use default %ds.\n", interval);
        }
    }

    run_cpu_probe(interval);
    return 0;
}
