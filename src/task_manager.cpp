#include "task_manager.h"

#include <iostream>

#include "logger.h"

TaskManager::TaskManager() : next_id_(1) { loadTasks(); }

void TaskManager::addTask(const std::string& description, int priority,
                          const std::string& date) {
    Task task;
    task.id_ = next_id_++;
    task.description_ = description;
    task.priority_ = priority;
    task.due_date_ = date;
    tasks_.emplace_back(task);
    Logger::getInstance().log("Task added: " + task.toString());
    saveTasks();
}

void TaskManager::removeTask(int id) {
    // std::find_if(begin,end,判断条件)
    // 它会从 [begin, end)
    // 范围里一个一个地用“判断条件”去检查每个元素，找到第一个满足条件的就停下，返回迭代器。
    auto it =
        std::find_if(tasks_.begin(), tasks_.end(),
                     [id](const Task& task) -> bool { return task.id_ == id; });

    if (it != tasks_.end()) {
        Logger::getInstance().log("Task removed: " + it->toString());
        tasks_.erase(it);
        saveTasks();
    } else {
        std::cout << "Task not found." << std::endl;
    }
}

void TaskManager::updateTask(int id, const std::string& description,
                             int priority, const std::string& date) {
    auto it =
        std::find_if(tasks_.begin(), tasks_.end(),
                     [id](const Task& task) -> bool { return task.id_ == id; });

    if (it != tasks_.end()) {
        it->description_ = description;
        it->priority_ = priority;
        it->due_date_ = date;
        Logger::getInstance().log("Task updated: " + it->toString());
        saveTasks();
    } else {
        std::cout << "Task not found." << std::endl;
    }
}

void TaskManager::saveTasks() const {
    // 默认清空文件重新写入
    std::ofstream outFile("data/tasks.txt");
    if (!outFile.is_open()) {
        Logger::getInstance().log("Failed to open tasks file for writing.");
        return;
    }

    for (const auto& task : tasks_) {
        outFile << task.id_ << "," << task.description_ << "," << task.priority_
                << "," << task.due_date_ << std::endl;
    }

    outFile.close();
    Logger::getInstance().log("Tasks saved successfully.");
}

void TaskManager::listTasks(int sort_option) const {
    std::vector<Task> sorted_tasks = tasks_;

    switch (sort_option) {
        case 1:
            std::sort(sorted_tasks.begin(), sorted_tasks.end(),
                      compareByPriority);
            break;
        case 2:
            std::sort(sorted_tasks.begin(), sorted_tasks.end(),
                      compareByDueDate);
            break;
        default:
            break;
    }

    for (const auto& task : sorted_tasks) {
        std::cout << task.toString() << std::endl;
    }
}

void TaskManager::loadTasks() {
    std::ifstream inFile("data/tasks.txt");
    if (!inFile.is_open()) {
        Logger::getInstance().log("Failed to open tasks file.");
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        Task task;
        char delimiter;  // 用于分隔符
        iss >> task.id_ >> delimiter;
        std::getline(iss, task.description_, ',');
        iss >> task.priority_ >> delimiter;
        iss >> task.due_date_;
        tasks_.emplace_back(task);
        if (task.id_ >= next_id_) {
            next_id_ = task.id_ + 1;
        }
    }

    // 可以不手动关闭，其支持RAII
    inFile.close();
    Logger::getInstance().log("Tasks loaded successfully.");
}

bool TaskManager::compareByPriority(const Task& a, const Task& b) {
    return a.priority_ < b.priority_;
}

bool TaskManager::compareByDueDate(const Task& a, const Task& b) {
    return a.due_date_ < b.due_date_;
}
