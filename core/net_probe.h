#ifndef NET_PROBE_H
#define NET_PROBE_H

typedef struct {
    long last_rx_bytes;
    long last_tx_bytes;
    long rx_bytes;
    long tx_bytes;
    double rx_rate;
    double tx_rate;
} net_probe_data_t;

Probe *get_net_probe(void);

#endif /* NET_PROBE_H */
