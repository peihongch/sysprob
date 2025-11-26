CC = gcc
CFLAGS = -Wall
PICFLAGS = -Wall -fPIC

OBJS = main.o \
	cmd/cpu_cmd.o cmd/mem_cmd.o cmd/disk_cmd.o cmd/net_cmd.o cmd/summary_cmd.o \
	core/cpu_probe.o core/probe.o core/mem_probe.o core/disk_probe.o core/net_probe.o core/summary.o \
	plugin/plugin.o \
	report/reporter.o report/text_reporter.o report/csv_reporter.o report/json_reporter.o \
	conf/config.o \
	util/procfs_util.o util/logger.o

all: sysprob cpu_usage_plugin.so mem_usage_plugin.so

sysprob: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -ldl

# 插件目标文件 - 使用 PIC 编译
PLUGIN_OBJS = plugin/cpu_usage_plugin.o plugin/mem_usage_plugin.o \
	core/probe_plugin.o core/cpu_probe_plugin.o core/mem_probe_plugin.o \
	util/logger_plugin.o util/procfs_util_plugin.o

cpu_usage_plugin.so: plugin/cpu_usage_plugin.o core/probe_plugin.o core/cpu_probe_plugin.o util/logger_plugin.o util/procfs_util_plugin.o
	$(CC) -shared -o $@ $^ -ldl

mem_usage_plugin.so: plugin/mem_usage_plugin.o core/probe_plugin.o core/mem_probe_plugin.o util/logger_plugin.o util/procfs_util_plugin.o
	$(CC) -shared -o $@ $^ -ldl

# 为插件创建 PIC 编译规则
%_plugin.o: %_plugin.c
	$(CC) $(PICFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(PLUGIN_OBJS) sysprob *.so

.PHONY: all clean