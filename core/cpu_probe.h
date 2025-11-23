#ifndef CPU_PROBE_H
#define CPU_PROBE_H

#include "../util/procfs_util.h"

typedef enum {
    CPU_USAGE_TOTAL,
    CPU_USAGE_USER
} cpu_usage_type_t;

typedef struct {
    struct cpu_time last_cpu_time;
    struct cpu_time current_cpu_time;
    double cpu_usage;
} cpu_probe_data_t;

Probe *get_cpu_probe(void);

#endif
