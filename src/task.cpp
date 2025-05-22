#include "task.h"

#include <iostream>
#include <sstream>

std::string Task::toString() const {
    // ostringstream 让你可以像对待 std::cout
    // 一样，把各种变量写进去，最终把内容组合成一个 std::string
    std::ostringstream oss;
    oss << "ID: " << id_ << ", Documentation: " << description_
        << ", Priority: " << priority_ << ", Due Date: " << due_date_;

    // 将其转换为string类型
    return oss.str();
}