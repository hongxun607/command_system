#pragma once

#include <iomanip>
#include <iostream>
#include <string>

class Task {
public:
    // 将任务的描述、ID、优先级和截止日期转换为一个string
    std::string toString() const;

    int id_;
    std::string description_;
    int priority_;
    std::string due_date_;
};