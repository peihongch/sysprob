#include <stdio.h>
#include <stdlib.h>

#include "probe.h"
#include "net_probe.h"
#include "../util/logger.h"
#include "../util/procfs_util.h"

static int net_init(Probe *self);
static int net_collect(Probe *self, ProbeOptions *options);
static int net_compute_metrics(Probe *self, ProbeOptions *options);
static int net_display(Probe *self, ProbeOptions *options);

typedef struct {
    long last_rx_bytes;
    long last_tx_bytes;
    long rx_bytes;
    long tx_bytes;
    double rx_rate;
    double tx_rate;
} net_probe_data_t;

static net_probe_data_t net_data;

static struct Probe net_probe = {
    .name = "Network Probe",
    .init = net_init,
    .collect = net_collect,
    .compute = net_compute_metrics,
    .display = net_display,
    .private_data = &net_data
};

Probe *get_net_probe(void) {
    return &net_probe;
}

static int net_init(Probe *self) {
    net_probe_data_t *data = (net_probe_data_t *)self->private_data;
    struct net_dev_stats stats;
    data->rx_bytes = 0;
    data->tx_bytes = 0;
    data->rx_rate = 0.0;
    data->tx_rate = 0.0;
    if (read_net_dev_stats(&stats) != 0) {
        LOG_ERROR("Failed to read network device statistics");
        return -1;
    }
    data->last_rx_bytes = stats.rx_bytes;
    data->last_tx_bytes = stats.tx_bytes;
    return 0;
}

static int net_collect(Probe *self, ProbeOptions *options) {
    net_probe_data_t *data = (net_probe_data_t *)self->private_data;
    struct net_dev_stats stats;

    if (read_net_dev_stats(&stats) != 0) {
        LOG_ERROR("Failed to read network device statistics");
        return -1;
    }

    data->rx_bytes = stats.rx_bytes;
    data->tx_bytes = stats.tx_bytes;
    return 0;
}

static inline double cal_net_rate(long last_bytes, long current_bytes, int interval) {
    return (double)(current_bytes - last_bytes) / interval;
}

static int net_compute_metrics(Probe *self, ProbeOptions *options) {
    net_probe_data_t *data = (net_probe_data_t *)self->private_data;
    int interval = options ? options->interval : 1;

    data->rx_rate = cal_net_rate(data->last_rx_bytes, data->rx_bytes, interval);
    data->tx_rate = cal_net_rate(data->last_tx_bytes, data->tx_bytes, interval);

    data->last_rx_bytes = data->rx_bytes;
    data->last_tx_bytes = data->tx_bytes;

    return 0;
}

static inline double net_rate_kbps(double rate_bytes) {
    return rate_bytes * 8.0 / 1024.0;
}

static int net_display(Probe *self, ProbeOptions *options) {
    net_probe_data_t *data = (net_probe_data_t *)self->private_data;
    printf("Network RX Rate: %.2f Kb/s, TX Rate: %.2f Kb/s\n",
              net_rate_kbps(data->rx_rate), net_rate_kbps(data->tx_rate));
    return 0;
}
