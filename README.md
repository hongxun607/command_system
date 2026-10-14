# command_system

一个使用 **C++17** 编写的交互式命令行任务管理器。任务包含 ID、描述、优先级和截止日期，支持新增、删除、修改、查看，以及本地文本持久化和日志记录。

项目也展示了命令分发的几种实现思路：当前实际启用的是 `CommandWrapper` 类型擦除包装，具体命令通过 CRTP 模板获得统一的 `execute` 接口；`main.cpp` 中还保留了虚基类、`std::function` 和 `std::variant` 方案的注释代码。

## 功能

- 新增任务，自动分配整数 ID
- 按 ID 删除或修改任务
- 按当前存储顺序、优先级或截止日期查看任务
- 启动时读取 `data/tasks.txt`，新增、成功删除或成功修改后保存
- 将操作日志追加到 `data/log.txt`

## 目录结构

```text
command_system/
├── CMakeLists.txt            # CMake 配置，生成 my_app
├── main.cpp                  # 交互循环、命令解析和注册
├── include/
│   ├── command.h             # CRTP 命令接口
│   ├── command_wrapper.h     # 类型擦除包装
│   ├── extra_command.h       # add/delete/list/update 命令声明
│   ├── logger.h              # 单例日志接口
│   ├── task.h                # 任务数据结构
│   └── task_manager.h        # 任务管理与持久化接口
├── src/
│   ├── extra_command.cpp     # 命令参数解析
│   ├── logger.cpp            # 带时间戳的追加日志
│   ├── task.cpp              # 任务显示文本
│   └── task_manager.cpp      # 增删改查、排序、文件读写
└── data/
    └── tasks.txt             # 已纳入版本控制的任务数据
```

`build/` 和运行时生成的 `data/log.txt` 已在 `.gitignore` 中忽略。

## 构建与启动

### 环境要求

- 支持 C++17 的 C++ 编译器
- CMake 3.10 或更新版本
- 对应 CMake 生成器需要的构建工具，例如 Make、Ninja 或 Visual Studio

项目当前没有声明第三方库依赖。以下命令依据现有 CMake 配置编写，本次文档更新未执行编译或运行验证。

### Linux / macOS 示例

```bash
git clone https://github.com/hongxun607/command_system.git
cd command_system
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..
./build/my_app
```

**请在仓库根目录启动程序，并确保 `data/` 存在且可写。** 程序使用相对路径 `data/tasks.txt` 和 `data/log.txt`，路径以当前工作目录为准。直接在 `build/` 内运行可能找不到数据目录，并因无法打开日志文件而抛出异常。

Windows 下同样可通过 CMake 配置、构建；可执行文件名为 `my_app.exe`，具体输出目录取决于生成器。使用多配置生成器时，可运行 `cmake --build . --config Debug`，再回到仓库根目录启动相应的 `build/Debug/my_app.exe`。

## 使用方式

启动后，在程序自己的交互提示符中输入命令。命令名区分大小写；命令与参数用空格分隔，多个任务字段用英文逗号分隔。

| 命令 | 用途 |
| --- | --- |
| `add <描述>,<优先级>,<截止日期>` | 新增任务 |
| `delete <任务ID>` | 删除指定任务 |
| `update <任务ID>,<描述>,<优先级>,<截止日期>` | 替换指定任务的描述、优先级和日期 |
| `list` 或 `list 0` | 按当前存储顺序列出任务 |
| `list 1` | 按优先级数值升序排列 |
| `list 2` | 按截止日期字符串升序排列 |
| `exit` | 退出程序 |

例如：

```text
add 完成 README,1,2026-10-20
list
list 1
list 2
```

先用 `list` 查看实际 ID。若需要修改的任务 ID 为 `2`，可执行：

```text
update 2,完善项目文档,2,2026-10-25
delete 2
exit
```

注意：

- 优先级使用整数；排序时数值越小越靠前，程序没有定义固定的优先级等级或范围。
- 日期建议统一写成 `YYYY-MM-DD`，例如 `2026-10-20`。当前实现按字符串排序，没有日期合法性校验。
- 描述可以包含空格，但不要包含英文逗号或换行；文件格式没有转义机制。
- 不需要为描述添加引号；引号会被当作描述的一部分保存。
- `list 0` 不会主动按 ID 重新排序，而是保留当前加载或添加的顺序。

## 数据与日志

### 任务文件

`data/tasks.txt` 每行保存一条任务，字段顺序为：

```text
ID,描述,优先级,截止日期
```

仓库已包含一条示例任务。程序启动时加载文件，并根据读到的最大 ID 确定下一个 ID；新增、成功删除或成功修改任务时，会重新写入整个任务文件。

维护数据时建议：

1. 先退出程序，再手动编辑或恢复 `data/tasks.txt`，避免后续保存覆盖手动修改。
2. 编辑前保留备份，并保持每行四个字段、整数 ID 和整数优先级的格式。
3. 提交前检查 `git diff -- data/tasks.txt`。该文件受 Git 跟踪，不要意外提交私人任务内容。

### 日志文件

`data/log.txt` 使用追加模式，记录带时间戳的操作信息。日志打开失败时会抛出异常；请先检查启动目录及 `data/` 的写入权限。当前没有日志轮转机制，长期使用时可在程序退出后按需归档日志。

## 维护与扩展

- **修改任务行为：** 主要查看 `src/task_manager.cpp`。
- **修改命令格式或校验：** 主要查看 `src/extra_command.cpp`。
- **新增命令：** 在 `include/extra_command.h` 声明命令类，实现 `executeImpl(const std::string&)`，在 `src/extra_command.cpp` 中实现，再在 `main.cpp` 的 `commands` 映射中注册。
- **新增源文件：** CMake 通过 `src/*.cpp` 收集实现文件。新增文件后重新运行 CMake 配置，再构建。
- **修改数据字段：** 同步检查 `Task`、命令解析、`loadTasks()`、`saveTasks()` 和 `toString()`，并考虑已有任务文件的兼容性。

修改代码后，建议在独立的测试副本中检查新增、查看、排序、修改、删除、退出和重启后加载，避免影响正式任务数据。仓库当前未提供自动化测试或 CI 配置。

## 当前限制与注意事项

以下内容根据现有源码整理，尚未通过本次文档更新进行运行验证：

- 输入与数据文件校验尚不完整。例如，`add` 的优先级转换没有捕获异常，非法或超范围的整数可能导致程序退出；读取任务文件时也没有逐行验证格式。
- 删除或修改不存在的 ID 时，任务管理层会输出 `Task not found.`，但命令层仍可能输出成功提示，应再用 `list` 核对结果。
- 交互循环尚未处理标准输入结束（EOF）；正常使用时请用 `exit` 退出。
- 部分源文件未显式包含其使用类型对应的标准头文件，例如 `src/task_manager.cpp` 使用 `std::istringstream`，但未直接包含 `<sstream>`。编译结果可能依赖间接包含的头文件；若出现相关错误，应在后续代码维护中补齐直接依赖。
- `CommandWrapper` 通过基类指针管理包装对象，但其内部 `Concept` 尚未声明虚析构函数，属于后续代码维护需要处理的对象生命周期问题。
