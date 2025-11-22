#include <stdio.h>
#include <unistd.h>

struct cpu_time {
    long user;
    long nice;
    long system;
    long idle;
};

static long get_cpu_time(struct cpu_time *cpu_time) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Unable to open /proc/stat");
        return -1;
    }

    // Read the first line, which contains CPU usage data
    if (fscanf(file, "cpu %ld %ld %ld %ld",
                     &cpu_time->user,
                     &cpu_time->nice,
                     &cpu_time->system,
                     &cpu_time->idle) != 4) {
        perror("Failed to read /proc/stat");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

static double sample_cpu_usage(int interval) {
    struct cpu_time prev, curr;

    get_cpu_time(&prev);
    sleep(interval);
    get_cpu_time(&curr);

    long busy = (curr.user + curr.nice + curr.system) - (prev.user + prev.nice + prev.system);
    long total = (curr.user + curr.nice + curr.system + curr.idle) - (prev.user + prev.nice + prev.system + prev.idle);

    return (double)busy / total * 100;
}

void run_cpu_probe(int interval) {
    while (1) {
        double usage = sample_cpu_usage(interval);
        printf("CPU: %.2f%%\n", usage);
    }
}
