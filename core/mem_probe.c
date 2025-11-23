#include <stdio.h>
#include <stdlib.h>

#include "probe.h"
#include "mem_probe.h"
#include "../util/procfs_util.h"
#include "../util/logger.h"

static int mem_init(Probe *self);
static int mem_collect(Probe *self, ProbeOptions *options);
static int mem_compute_metrics(Probe *self, ProbeOptions *options);
static int mem_display(Probe *self, ProbeOptions *options);

typedef struct {
    long total_mem_kb;
    long free_mem_kb;
    double mem_usage_percent;
} mem_probe_data_t;

static mem_probe_data_t mem_data;

static struct Probe mem_probe = {
    .name = "Memory Probe",
    .init = mem_init,
    .collect = mem_collect,
    .compute = mem_compute_metrics,
    .display = mem_display,
    .private_data = &mem_data
};

Probe *get_mem_probe(void) {
    return &mem_probe;
}

static inline double cal_mem_usage(mem_probe_data_t *data) {
    double mem_usage_percent;

    if (data->total_mem_kb > 0) {
        mem_usage_percent = ((double)(data->total_mem_kb - data->free_mem_kb) /
                             data->total_mem_kb) * 100.0;
    } else {
        LOG_WARN("Total memory is zero, cannot compute usage percentage");
        mem_usage_percent = 0.0;
    }

    return mem_usage_percent;
}

static int mem_init(Probe *self) {
    mem_probe_data_t *data = (mem_probe_data_t *)self->private_data;
    data->total_mem_kb = 0;
    data->free_mem_kb = 0;
    data->mem_usage_percent = 0.0;
    return 0;
}

static int mem_collect(Probe *self, ProbeOptions *options) {
    mem_probe_data_t *data = (mem_probe_data_t *)self->private_data;
    struct mem_info mem_info;

    if (read_mem_info(&mem_info) != 0) {
        LOG_ERROR("Failed to read memory info from /proc/meminfo");
        return -1;
    }

    data->total_mem_kb = mem_info.total_kb;
    data->free_mem_kb = mem_info.free_kb;

    return 0;
}

static int mem_compute_metrics(Probe *self, ProbeOptions *options) {
    mem_probe_data_t *data = (mem_probe_data_t *)self->private_data;

    if (data->total_mem_kb == 0) {
        LOG_WARN("Total memory is zero, cannot compute usage percentage");
        data->mem_usage_percent = 0.0;
        return 0;
    }

    data->mem_usage_percent = cal_mem_usage(data);

    return 0;
}

static int mem_display(Probe *self, ProbeOptions *options) {
    mem_probe_data_t *data = (mem_probe_data_t *)self->private_data;
    printf("Memory Usage: %.2f%% (Total: %ld kB, Free: %ld kB)\n",
           data->mem_usage_percent,
           data->total_mem_kb,
           data->free_mem_kb);
    LOG_INFO("Memory Usage: %.2f%% (Total: %ld kB, Free: %ld kB)",
             data->mem_usage_percent,
             data->total_mem_kb,
             data->free_mem_kb);
    return 0;
}
