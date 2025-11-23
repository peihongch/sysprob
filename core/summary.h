#ifndef SUMMARY_H
#define SUMMARY_H

#include "../conf/config.h"

typedef struct {
    struct {
        double total;
        double user;
    } cpu_usage;
    struct {
        long total_kb;
        long free_kb;
    } mem_usage;
    struct {
        long total_kb;
        long free_kb;
    } disk_usage;
    struct {
        long rx_bytes;
        long tx_bytes;
    } net_rate;
} SystemSummary;

void print_system_summary(config_t *config);

#endif /* SUMMARY_H */
