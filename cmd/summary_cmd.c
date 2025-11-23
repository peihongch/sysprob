
#include "summary_cmd.h"
#include "../core/summary.h"

int run_summary_cmd(config_t *config, int argc, char *argv[]) {
    print_system_summary(config);
    return 0;
}
