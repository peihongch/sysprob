#ifndef PROBE_H
#define PROBE_H

typedef struct ProbeOptions {
    int interval;
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

#endif
