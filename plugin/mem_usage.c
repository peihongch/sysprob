#include "plugin.h"
#include "../core/mem_probe.h"

static Plugin mem_usage_plugin = {
    .name = "mem_usage",
};

Plugin *sysprob_plugin(void) {
    mem_usage_plugin.probe = get_mem_probe();
    return &mem_usage_plugin;
}
