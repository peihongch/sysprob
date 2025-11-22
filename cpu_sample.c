#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct cpu_time {
    long user;
    long nice;
    long system;
    long idle;
};

long get_cpu_time(struct cpu_time *cpu_time) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Unable to open /proc/stat");
        return -1;
    }

    // Read the first line, which contains CPU usage data
    if (fscanf(file, "cpu %ld %ld %ld %ld", &cpu_time->user, &cpu_time->nice, &cpu_time->system, &cpu_time->idle) != 4) {
        perror("Failed to read /proc/stat");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <sampling_interval>\n", argv[0]);
        return 1;
    }

    int interval = atoi(argv[1]);
    if (interval <= 0) {
        fprintf(stderr, "Interval must be a positive number.\n");
        return 1;
    }

    struct cpu_time prev, curr;
    while (1) {
        get_cpu_time(&prev);
        sleep(interval);
        get_cpu_time(&curr);

        long busy = (curr.user + curr.nice + curr.system) - (prev.user + prev.nice + prev.system);
        long total = (curr.user + curr.nice + curr.system + curr.idle) - (prev.user + prev.nice + prev.system + prev.idle);

        if (total > 0) {
            printf("CPU: %.2f%%\n", (double)busy / total * 100);
        }
    }

    return 0;
}