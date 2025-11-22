#ifndef CPU_PROBE_H
#define CPU_PROBE_H

typedef enum {
    CPU_USAGE_TOTAL,
    CPU_USAGE_USER
} cpu_usage_type_t;

long probe_cpu_usage(int interval, cpu_usage_type_t type);

#endif
