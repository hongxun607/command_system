#pragma once

#include <iostream>
#include <memory>

#include "extra_command.h"

class CommandWrapper {
public:
    template <typename T>
    CommandWrapper(T cmd) : impl_(std::make_unique<Model<T>>(std::move(cmd))){};
    void execute(const std::string& args) { impl_->execute(args); };

private:
    struct Concept {
        virtual void execute(const std::string& args) = 0;
    };

    template <typename T>
    struct Model : public Concept {
        T command_;
        Model(T cmd) : command_(std::move(cmd)) {}
        void execute(const std::string& args) { command_.execute(args); }
    };

    std::unique_ptr<Concept> impl_;
};