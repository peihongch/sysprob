#include <unistd.h>

#include "probe.h"
#include "../util/logger.h"

int run_probe(Probe *probe, ProbeOptions *options) {
    int interval = options ? options->interval : 1;

    if (!probe) {
        LOG_ERROR("Probe is NULL");
        return -1;
    }

    LOG_INFO("Starting probe %s with interval %d", probe->name, interval);

    if (probe->init) {
        if (probe->init(probe) != 0) {
            LOG_ERROR("Failed to initialize probe %s", probe->name);
            return -1;
        }
    }

    while (1) {
        sleep(interval);

        if (run_probe_once(probe, options, true) != 0) {
            LOG_ERROR("Probe %s encountered an error during execution", probe->name);
            return -1;
        }
    }

    return 0;
}

int run_probe_once(Probe *probe, ProbeOptions *options, bool output) {
    if (!probe) {
        LOG_ERROR("Probe is NULL");
        return -1;
    }

    LOG_INFO("Running probe %s once", probe->name);

    if (probe->collect)
        if (probe->collect(probe, options) != 0) {
            LOG_ERROR("Failed to collect data for probe %s", probe->name);
            return -1;
        }
    if (probe->compute)
        if (probe->compute(probe, options) != 0) {
            LOG_ERROR("Failed to compute metrics for probe %s", probe->name);
            return -1;
        }
    if (probe->display && output) {
        if (probe->display(probe, options) != 0) {
            LOG_ERROR("Failed to display data for probe %s", probe->name);
            return -1;
        }
    }

    return 0;
}
