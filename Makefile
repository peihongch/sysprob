CC = gcc
CFLAGS = -Wall

OBJS = main.o \
	cmd/cpu_cmd.o cmd/mem_cmd.o cmd/disk_cmd.o cmd/net_cmd.o \
	core/cpu_probe.o core/probe.o core/mem_probe.o core/disk_probe.o core/net_probe.o \
	plugin/plugin.o \
	report/reporter.o report/text_reporter.o report/csv_reporter.o report/json_reporter.o \
	conf/config.o \
	util/procfs_util.o util/logger.o

all: sysprob cpu_usage_plugin.so mem_usage_plugin.so

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -ldl

CPU_USAGE_PLUGIN_OBJS = plugin/cpu_usage.o core/probe.o core/cpu_probe.o \
	util/logger.o util/procfs_util.o

cpu_usage_plugin.so: $(CPU_USAGE_PLUGIN_OBJS)
	$(CC) -shared -o $@ $(CPU_USAGE_PLUGIN_OBJS) -ldl

MEM_USAGE_PLUGIN_OBJS = plugin/mem_usage.o core/probe.o core/mem_probe.o \
	util/logger.o util/procfs_util.o

mem_usage_plugin.so: $(MEM_USAGE_PLUGIN_OBJS)
	$(CC) -shared -o $@ $(MEM_USAGE_PLUGIN_OBJS) -ldl

clean:
	rm -f $(OBJS) $(CPU_USAGE_PLUGIN_OBJS) $(MEM_USAGE_PLUGIN_OBJS) sysprob *.so
