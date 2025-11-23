#include "plugin.h"
#include "../core/cpu_probe.h"

static Plugin cpu_usage_plugin = {
    .name = "cpu_usage",
};

Plugin *sysprob_plugin(void) {
    cpu_usage_plugin.probe = get_cpu_probe();
    return &cpu_usage_plugin;
}
