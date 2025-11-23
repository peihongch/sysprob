#include <stdio.h>
#include <string.h>

#include "cmd/cpu_cmd.h"
#include "cmd/mem_cmd.h"
#include "cmd/disk_cmd.h"
#include "cmd/net_cmd.h"
#include "util/logger.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: sysprob cpu [interval]\n");
        return 0;
    }

    if (strcmp(argv[1], "cpu") == 0) {
        return run_cpu_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "mem") == 0) {
        return run_mem_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "disk") == 0) {
        return run_disk_cmd(argc - 1, argv + 1);
    } else if (strcmp(argv[1], "net") == 0) {
        return run_net_cmd(argc - 1, argv + 1);
    }

    LOG_ERROR("Unknown command: %s", argv[1]);
    return 0;
}