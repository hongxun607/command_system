#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

class Logger {
public:
    // 获取单例
    static Logger& getInstance();
    // 禁止拷贝和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    // 记录日志
    void log(const std::string& message);
    // 析构
    ~Logger();

private:
    // 私有构造函数，防止外部创建
    Logger();
    std::ofstream logFile_;
    std::mutex mutex_;
};