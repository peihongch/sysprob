#ifndef PROBE_H
#define PROBE_H

#include <stdbool.h>

#include "../report/reporter.h"

typedef struct ProbeOptions {
    int interval;
    reporter_format_t report_format;
    void *extra;
} ProbeOptions;

typedef struct Probe {
    const char *name;
    int (*init)(struct Probe *self);
    int (*collect)(struct Probe *self, ProbeOptions *options);
    int (*compute)(struct Probe *self, ProbeOptions *options);
    int (*display)(struct Probe *self, ProbeOptions *options);
    void *private_data;
} Probe;

int run_probe(Probe *probe, ProbeOptions *options);
int run_probe_once(Probe *probe, ProbeOptions *options, bool output);

#endif
