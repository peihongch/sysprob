#ifndef DISK_PROBE_H
#define DISK_PROBE_H

typedef struct {
    long root_total_kb;
    long root_free_kb;
    double root_usage_percent;
} disk_probe_data_t;

Probe *get_disk_probe(void);

#endif /* DISK_PROBE_H */
