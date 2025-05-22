#pragma once

#include <iostream>

// class CommandBase {
// public:
//     virtual void execute(const std::string& args) = 0;
// };

template <typename Derived>
class Command {
public:
    void execute(const std::string& args) {
        static_cast<Derived*>(this)->executeImpl(args);
    }
};