**Project Overview**

sysprob 是一个轻量级的系统资源采样/探测工具（用 C 实现），用于周期性读取并展示主机的各类资源使用率（CPU、内存、磁盘、网络等）。项目采用模块化设计，支持通过插件扩展探测器。

**Quick Summary**
* **Language:** C
* **Platform:** Linux (依赖 `/proc` 系统接口)
* **Build:** `Makefile` 或 `gcc`（见下文）

**Features**
* **CPU/Mem/Disk/Net probes:** 模块化的探测器位于 `core/` 目录。
* **插件机制:** 可在 `~/.sysprob/plugins/` 放置共享库（`.so`）以动态扩展功能，程序启动时会扫描并加载这些插件。
* **可组合命令行工具:** 程序支持按采样间隔运行并打印结果，便于在监控或脚本中使用。

**Repository Layout**
* `cmd/`: 子命令入口以及参数解析。
* `conf/`: 配置文件解析和加载。
* `core/`: 核心探测器实现（例如 `cpu_probe.c`、`mem_probe.c`、`summary.c` 等）。
* `plugin/`: 插件加载器实现与插件相关代码（例如 `plugin.c`）。
* `report/`: 监控指标的结构化输出。
* `util/`: 日志与工具函数（如 `logger.h`/`.c`）。
* 顶层源文件: `main.c`。

**Build Instructions**
1. 使用 `Makefile`（推荐）：

```bash
make
```

2. 或者手动编译单个模块（示例）：

```bash
gcc -Wall -Wextra -c core/procfs.c -o core/procfs.o
gcc -Wall -Wextra -c core/cpu_probe.c -o core/cpu_probe.o
gcc core/procfs.o core/cpu_probe.o -o sysprob
```

注意：不同模块可能需要额外的源文件一起编译（比如 `util/logger.c`、其他 probe 实现等）。

**Run Examples**
* 运行 CPU 采样（按项目实际入口进行替换）：

```bash
./sysprob cpu 1    # 每 1 秒采样一次 CPU（示例用法）
```

* 获取系统层级监控指标汇总：

```bash
./sysprob summary  # 汇总输出全部的系统监控指标
```

**Plugins**
* 插件目录默认配置为 `~/.sysprob/plugins/`。程序会在启动时扫描该目录并尝试加载所有带有 `.so` 后缀的共享库。
* 如果启动日志中看到类似 `Plugin directory ~/.sysprob/plugins/ not found or inaccessible`，请确认：
  * 目录存在且可读：`mkdir -p ~/.sysprob/plugins && chmod 755 ~/.sysprob/plugins`
  * 插件为共享库，导出 `sysprob_plugin` 符号（函数返回 `Plugin *`）。
* 示例安装插件：

```bash
# 将编译好的 my_plugin.so 放到插件目录
cp my_plugin.so ~/.sysprob/plugins/
```

**Core API / Extension Points**
* `core/*_probe.c`: 各类资源探针实现（每个 probe 提供采样和 run 接口）。
* `report/*_reporter.c`: 各类结构化输出的实现。
* `plugin/plugin.c`: 插件加载与注册逻辑。插件需实现约定的 `sysprob_plugin()` 工厂函数并返回 `Plugin` 结构。

**Troubleshooting**
* `/proc/stat` 读取失败：程序会打印 `Unable to open /proc/stat`，通常是权限或系统异常，请确保在 Linux 环境运行且当前用户可以读取 `/proc/stat`（普通用户通常可读）。
* 插件加载失败：查看日志中的 `dlopen` / `dlsym` 错误信息，确保插件与主程序的 ABI/依赖兼容（相同的 libc/编译选项更安全）。

**Development & Testing**
* 建议在本机上使用 `gcc -Wall -Wextra -g` 编译。
* 提交代码前，请运行 `make clean` + `make` 并修复警告。

**Contributing**
* 欢迎提交 PR：请在 PR 描述中包含变更摘要和简单的测试步骤。
* 请保持代码风格一致，函数修改应附带必要的错误处理。

**Acknowledgements & Notes**
* 该工具直接读取 `/proc`，适用于 Linux 系统。
* 如果需要将输出发送到远端或持久化，建议在上层包装脚本或添加 output 模块（例如 JSON 输出或发送到 HTTP/InfluxDB）。
