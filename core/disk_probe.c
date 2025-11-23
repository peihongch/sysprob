#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>

#include "probe.h"
#include "disk_probe.h"
#include "../util/log_util.h"

static int disk_init(Probe *self);
static int disk_collect(Probe *self, ProbeOptions *options);
static int disk_compute_metrics(Probe *self, ProbeOptions *options);
static int disk_display(Probe *self, ProbeOptions *options);

typedef struct {
    long root_total_kb;
    long root_free_kb;
    double root_usage_percent;
} disk_probe_data_t;

static disk_probe_data_t disk_data;

static struct Probe disk_probe = {
    .name = "Disk Probe",
    .init = disk_init,
    .collect = disk_collect,
    .compute = disk_compute_metrics,
    .display = disk_display,
    .private_data = &disk_data
};

Probe *get_disk_probe(void) {
    return &disk_probe;
}

static int disk_init(Probe *self) {
    disk_probe_data_t *data = (disk_probe_data_t *)self->private_data;
    data->root_total_kb = 0;
    data->root_free_kb = 0;
    data->root_usage_percent = 0.0;
    return 0;
}

static int disk_collect(Probe *self, ProbeOptions *options) {
    disk_probe_data_t *data = (disk_probe_data_t *)self->private_data;
    struct statvfs vfs;

    if (statvfs("/", &vfs) != 0) {
        LOG_ERROR("Failed to get filesystem statistics for /");
        return -1;
    }

    data->root_total_kb = (vfs.f_blocks * vfs.f_frsize) / 1024;
    data->root_free_kb = (vfs.f_bfree * vfs.f_frsize) / 1024;
    return 0;
}

static int disk_compute_metrics(Probe *self, ProbeOptions *options) {
    disk_probe_data_t *data = (disk_probe_data_t *)self->private_data;
    if (data->root_total_kb > 0) {
        data->root_usage_percent = ((double)(data->root_total_kb - data->root_free_kb) /
                                    data->root_total_kb) * 100.0;
    } else {
        LOG_WARN("Total disk size is zero, cannot compute usage percentage");
        data->root_usage_percent = 0.0;
    }
    return 0;
}

static int disk_display(Probe *self, ProbeOptions *options) {
    disk_probe_data_t *data = (disk_probe_data_t *)self->private_data;
    printf("Disk Usage for /: Total: %ld kB, Free: %ld kB, Usage: %.2f%%\n",
           data->root_total_kb, data->root_free_kb, data->root_usage_percent);
    return 0;
}
