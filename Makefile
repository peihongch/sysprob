CC = gcc
CFLAGS = -Wall

OBJS = main.o cmd/cpu_cmd.o core/cpu_probe.o

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) sysprob
