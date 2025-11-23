CC = gcc
CFLAGS = -Wall

OBJS = main.o \
	cmd/cpu_cmd.o cmd/mem_cmd.o cmd/disk_cmd.o \
	core/cpu_probe.o core/probe.o core/mem_probe.o core/disk_probe.o \
	util/procfs_util.o util/log_util.o

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) sysprob
