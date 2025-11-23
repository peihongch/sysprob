#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>

#include "plugin.h"
#include "../core/cpu_probe.h"
#include "../core/mem_probe.h"
#include "../core/disk_probe.h"
#include "../core/net_probe.h"
#include "../util/logger.h"

#define MAX_PLUGINS 100
#define PLUGIN_DIR "/root/.sysprob/plugins/"

static Plugin *plugins[MAX_PLUGINS];
static int plugin_count = 0;

static int register_plugin(Plugin *plugin) {
    if (plugin_count >= MAX_PLUGINS) {
        LOG_ERROR("Maximum number of plugins reached");
        return -1;
    }
    plugins[plugin_count++] = plugin;
    LOG_INFO("Registered plugin: %s", plugin->name);
    return 0;
}

static Plugin *find_plugin_by_name(const char *name) {
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i]->name, name) == 0) {
            return plugins[i];
        }
    }
    LOG_WARN("Plugin %s not found", name);
    return NULL;
}

static int load_single_plugin(const char *path) {
    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        LOG_ERROR("Failed to load plugin %s: %s", path, dlerror());
        return -1;
    }
    Plugin *(*get_plugin_fn)(void) = dlsym(handle, "sysprob_plugin");
    if (!get_plugin_fn) {
        LOG_ERROR("Failed to find sysprob_plugin in %s: %s", path, dlerror());
        dlclose(handle);
        return -1;
    }
    Plugin *plugin = get_plugin_fn();
    if (register_plugin(plugin) != 0) {
        dlclose(handle);
        return -1;
    }
    LOG_INFO("Loaded plugin: %s", plugin->name);
    return 0;
}

// Load additional plugins from shared libraries
int load_plugins(void) {
    DIR *dir = opendir(PLUGIN_DIR);
    if (!dir) {
        LOG_WARN("Plugin directory %s not found or inaccessible", PLUGIN_DIR);
        return 0; // No plugins to load
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".so")) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", PLUGIN_DIR, entry->d_name);
            if (load_single_plugin(path) != 0) {
                LOG_ERROR("Failed to load plugin from %s", path);
            }
        }
    }
    closedir(dir);

    LOG_INFO("Total plugins loaded: %d", plugin_count);

    return 0;
}

int run_plugin(const char *plugin_name, config_t *config, int argc, char *argv[]) {
    Plugin *plugin = find_plugin_by_name(plugin_name);
    if (!plugin) {
        return -1;
    }

    ProbeOptions options;
    options.interval = config->interval;
    options.extra = NULL;

    if (argc >= 2) {
        options.interval = atoi(argv[1]);
        if (options.interval <= 0) {
            options.interval = 1;
        }
    }

    return run_probe(plugin->probe, &options);
}
