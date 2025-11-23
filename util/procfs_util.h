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

struct mem_info {
    long total_kb;
    long free_kb;
};

/*
 * Read memory info from /proc/meminfo.
 * Returns 0 on success, -1 on failure.
 */
int read_mem_info(struct mem_info *mem_info);

#endif /* CORE_PROCFS_H */
