#ifndef PLUGIN_H
#define PLUGIN_H

#include "../core/probe.h"
#include "../conf/config.h"

typedef struct Plugin {
    const char *name;
    Probe *probe;
} Plugin;

Plugin *sysprob_plugin(void);

int load_plugins(void);
int run_plugin(const char *plugin_name, config_t *config, int argc, char *argv[]);

#endif /* PLUGIN_H */
