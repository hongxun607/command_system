#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "task.h"

class TaskManager {
public:
    TaskManager();
    ~TaskManager() = default;

    // 添加命令
    void addTask(const std::string& description, int priority,
                 const std::string& date);
    // 删除命令
    void removeTask(int id);
    // 更新命令
    void updateTask(int id, const std::string& description, int priority,
                    const std::string& date);
    // 列出命令
    void listTasks(int sort_option) const;  // 0.按ID 1.按优先级 2.按截止日期
    // 加载命令
    void loadTasks();
    // 保存命令
    void saveTasks() const;

private:
    std::vector<Task> tasks_;
    int next_id_;
    static bool compareByPriority(const Task& a, const Task& b);
    static bool compareByDueDate(const Task& a, const Task& b);
};