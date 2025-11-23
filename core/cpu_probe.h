#ifndef CPU_PROBE_H
#define CPU_PROBE_H

typedef enum {
    CPU_USAGE_TOTAL,
    CPU_USAGE_USER
} cpu_usage_type_t;

Probe *get_cpu_probe(void);

#endif
