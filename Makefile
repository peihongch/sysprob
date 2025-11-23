CC = gcc
CFLAGS = -Wall

OBJS = main.o \
	cmd/cpu_cmd.o core/cpu_probe.o core/probe.o \
	util/procfs_util.o util/log_util.o

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) sysprob
