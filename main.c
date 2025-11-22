#include <stdio.h>
#include <string.h>
#include "cmd/cpu_cmd.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: sysprob cpu [interval]\n");
        return 0;
    }

    if (strcmp(argv[1], "cpu") == 0) {
        return run_cpu_cmd(argc - 1, argv + 1);
    }

    printf("Unknown command: %s\n", argv[1]);
    return 0;
}