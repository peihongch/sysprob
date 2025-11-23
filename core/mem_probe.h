#ifndef MEM_PROBE_H
#define MEM_PROBE_H

typedef struct {
    long total_mem_kb;
    long free_mem_kb;
    double mem_usage_percent;
} mem_probe_data_t;

Probe *get_mem_probe(void);

#endif /* MEM_PROBE_H */
