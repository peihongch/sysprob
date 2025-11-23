#include <string.h>
#include <unistd.h>
#include <time.h>

#include "summary.h"
#include "probe.h"
#include "cpu_probe.h"
#include "mem_probe.h"
#include "disk_probe.h"
#include "net_probe.h"

static void collect_system_summary(SystemSummary *summary, ProbeOptions *options) {
    memset(summary, 0, sizeof(SystemSummary));

    // Collect CPU usage
    Probe *cpu_probe = get_cpu_probe();
    cpu_probe_data_t *cpu_data = (cpu_probe_data_t *)cpu_probe->private_data;
    if (cpu_probe->init)
    cpu_probe->init(cpu_probe);

    // Collect Network rate
    Probe *net_probe = get_net_probe();
    net_probe_data_t *net_data = (net_probe_data_t *)net_probe->private_data;
    if (net_probe->init)
        net_probe->init(net_probe);

    // Collect Disk usage
    Probe *disk_probe = get_disk_probe();
    disk_probe_data_t *disk_data = (disk_probe_data_t *)disk_probe->private_data;
    if (disk_probe->init)
        disk_probe->init(disk_probe);

    // Collect Memory usage
    Probe *mem_probe = get_mem_probe();
    mem_probe_data_t *mem_data = (mem_probe_data_t *)mem_probe->private_data;
    if (mem_probe->init)
        mem_probe->init(mem_probe);

    sleep(options->interval);
    options->extra = (void *)CPU_USAGE_TOTAL;
    run_probe_once(cpu_probe, options, false);
    summary->cpu_usage.total = cpu_data->cpu_usage;
    run_probe_once(mem_probe, options, false);
    run_probe_once(disk_probe, options, false);
    run_probe_once(net_probe, options, false);

    sleep(options->interval);
    options->extra = (void *)CPU_USAGE_USER;
    run_probe_once(cpu_probe, options, false);
    summary->cpu_usage.user = cpu_data->cpu_usage;
    
    // Populate summary data
    summary->mem_usage.total_kb = mem_data->total_mem_kb;
    summary->mem_usage.free_kb = mem_data->free_mem_kb;
    summary->disk_usage.total_kb = disk_data->root_total_kb;
    summary->disk_usage.free_kb = disk_data->root_free_kb;
    summary->net_rate.rx_bytes = net_data->rx_rate;
    summary->net_rate.tx_bytes = net_data->tx_rate;
}

static inline report_data_t get_summary_data(SystemSummary *summary) {
    report_data_t data = {
        .title = "System Summary",
        .entries = {
            [0] = {
                .key = "CPU Usage Total",
                .value = summary->cpu_usage.total,
                .value_suffix = "%",
            },
            [1] = {
                .key = "CPU Usage User",
                .value = summary->cpu_usage.user,
                .value_suffix = "%",
            },
            [2] = {
                .key = "Memory Total",
                .value = summary->mem_usage.total_kb,
                .value_suffix = " kB",
            },
            [3] = {
                .key = "Memory Free",
                .value = summary->mem_usage.free_kb,
                .value_suffix = " kB",
            },
            [4] = {
                .key = "Memory Usage",
                .value = (1 - (double)summary->mem_usage.free_kb / summary->mem_usage.total_kb) * 100,
                .value_suffix = "%",
            },
            [5] = {
                .key = "Disk Total",
                .value = summary->disk_usage.total_kb,
                .value_suffix = " kB",
            },
            [6] = {
                .key = "Disk Free",
                .value = summary->disk_usage.free_kb,
                .value_suffix = " kB",
            },
            [7] = {
                .key = "Disk Usage",
                .value = (1 - (double)summary->disk_usage.free_kb / summary->disk_usage.total_kb) * 100,
                .value_suffix = "%",
            },
            [8] = {
                .key = "Network RX Bytes",
                .value = summary->net_rate.rx_bytes,
                .value_suffix = " bytes",
            },
            [9] = {
                .key = "Network TX Bytes",
                .value = summary->net_rate.tx_bytes,
                .value_suffix = " bytes",
            },
        },
        .num_entries = 8,
    };
    return data;
}

void print_system_summary(config_t *config) {
    reporter_format_t format = REPORTER_FORMAT_TEXT;
    report_data_t summary_data;
    SystemSummary summary;
    ProbeOptions options = {
        .interval = config->interval,
        .report_format = REPORTER_FORMAT_TEXT,
        .extra = NULL
    };

    if (config->output_format != NULL) {
        if (strcmp(config->output_format, "csv") == 0) {
            format = REPORTER_FORMAT_CSV;
        } else if (strcmp(config->output_format, "json") == 0) {
            format = REPORTER_FORMAT_JSON;
        } else {
            format = REPORTER_FORMAT_TEXT;
        }
    }

    options.report_format = format;
    collect_system_summary(&summary, &options);
    summary_data = get_summary_data(&summary);
    report_data(get_reporter(format), &summary_data);
}
