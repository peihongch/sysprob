#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "probe.h"
#include "cpu_probe.h"
#include "../util/procfs_util.h"
#include "../util/logger.h"
#include "../report/reporter.h"

static int cpu_init(Probe *self);
static int cpu_collect(Probe *self, ProbeOptions *options);
static int cpu_compute_metrics(Probe *self, ProbeOptions *options);
static int cpu_display(Probe *self, ProbeOptions *options);

static cpu_probe_data_t cpu_data;

static struct Probe cpu_probe = {
    .name = "CPU Probe",
    .init = cpu_init,
    .collect = cpu_collect,
    .compute = cpu_compute_metrics,
    .display = cpu_display,
    .private_data = &cpu_data
};

static inline double cpu_time_total(struct cpu_time *ct) {
    return ct->user + ct->nice + ct->system + ct->idle;
}

static inline long cpu_time_busy(struct cpu_time *ct) {
    return ct->user + ct->nice + ct->system;
}

static double cal_cpu_usage(struct cpu_time prev, struct cpu_time curr) {
    long busy = cpu_time_busy(&curr) - cpu_time_busy(&prev);
    long total = cpu_time_total(&curr) - cpu_time_total(&prev);

    if (total <= 0) {
        LOG_WARN("Total CPU time difference is non-positive: %ld", total);
        return 0.0;
    }

    return (double)busy / total * 100.0;
}

static double cal_cpu_user_usage(struct cpu_time prev, struct cpu_time curr) {
    long user = curr.user - prev.user;
    long total = cpu_time_total(&curr) - cpu_time_total(&prev);

    if (total <= 0) {
        LOG_WARN("Total CPU time difference is non-positive: %ld", total);
        return 0.0;
    }

    return (double)user / total * 100.0;
}

static int cpu_init(struct Probe *self) {
    cpu_probe_data_t *data = (cpu_probe_data_t *)self->private_data;
    data->last_cpu_time = (struct cpu_time){0};
    data->cpu_usage = 0.0;

    read_cpu_time(&data->last_cpu_time);
    return 0;
}

static int cpu_collect(Probe *self, ProbeOptions *options) {
    cpu_probe_data_t *data = (cpu_probe_data_t *)self->private_data;
    struct cpu_time current_cpu_time;

    if (read_cpu_time(&current_cpu_time) != 0) {
        LOG_ERROR("Failed to read CPU time");
        return -1;
    }

    data->current_cpu_time = current_cpu_time;
    return 0;
}

static int cpu_compute_metrics(Probe *self, ProbeOptions *options) {
    cpu_usage_type_t type = (cpu_usage_type_t)options->extra;
    cpu_probe_data_t *data = (cpu_probe_data_t *)self->private_data;

    switch (type) {
    case CPU_USAGE_TOTAL:
        data->cpu_usage = cal_cpu_usage(data->last_cpu_time, data->current_cpu_time);
        break;
    case CPU_USAGE_USER:
        data->cpu_usage = cal_cpu_user_usage(data->last_cpu_time, data->current_cpu_time);
        break;
    }

    data->last_cpu_time = data->current_cpu_time;
    return 0;
}

static int cpu_display(Probe *self, ProbeOptions *options) {
    cpu_usage_type_t type = (cpu_usage_type_t)options->extra;
    cpu_probe_data_t *data = (cpu_probe_data_t *)self->private_data;
    reporter_format_t format = options->report_format;
    report_data_t cpu_usage_data = {
        .title = "CPU Usage",
        .entries = {
            [0] = {
                .value = data->cpu_usage,
                .value_suffix = "%",
            }
        },
        .num_entries = 1,
    };

    switch (type) {
    case CPU_USAGE_TOTAL:
        cpu_usage_data.entries[0].key = "Total";
        break;
    case CPU_USAGE_USER:
        cpu_usage_data.entries[0].key = "User";
        break;
    }

    report_data(get_reporter(format), &cpu_usage_data);
    return 0;
}

Probe *get_cpu_probe(void) {
    return &cpu_probe;
}
