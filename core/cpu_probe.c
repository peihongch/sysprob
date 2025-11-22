#include <stdio.h>
#include <unistd.h>

#include "cpu_probe.h"
#include "../util/procfs_util.h"
#include "../util/log_util.h"

static struct cpu_time prev = {0};

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

long probe_cpu_usage(int interval, cpu_usage_type_t type) {
    struct cpu_time curr;
    double usage;

    read_cpu_time(&prev);
    sleep(interval);
    read_cpu_time(&curr);

    if (type == CPU_USAGE_TOTAL) {
        usage = cal_cpu_usage(prev, curr);
    } else if (type == CPU_USAGE_USER) {
        usage = cal_cpu_user_usage(prev, curr);
    }

    prev = curr;
    return usage;
}
