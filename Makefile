CC = gcc
CFLAGS = -Wall

OBJS = main.o \
	cmd/cpu_cmd.o cmd/mem_cmd.o cmd/disk_cmd.o cmd/net_cmd.o \
	core/cpu_probe.o core/probe.o core/mem_probe.o core/disk_probe.o core/net_probe.o \
	plugin/plugin.o \
	util/procfs_util.o util/logger.o

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -ldl

clean:
	rm -f $(OBJS) sysprob
