#ifndef CORE_PROCFS_H
#define CORE_PROCFS_H

struct cpu_time {
    long user;
    long nice;
    long system;
    long idle;
};

/*
 * Read CPU times from /proc/stat (first "cpu" line).
 * Returns 0 on success, -1 on failure.
 */
int read_cpu_time(struct cpu_time *cpu_time);

#endif /* CORE_PROCFS_H */
