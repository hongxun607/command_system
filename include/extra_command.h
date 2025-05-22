#pragma once

#include <iostream>
#include <string>

#include "command.h"
#include "logger.h"
#include "task_manager.h"

class AddCommand : public Command<AddCommand> {
public:
    AddCommand(TaskManager& manager);
    void executeImpl(const std::string& args);

private:
    TaskManager& task_manager_;
};

class DeleteCommand : public Command<DeleteCommand> {
public:
    DeleteCommand(TaskManager& manager);
    void executeImpl(const std::string& args);

private:
    TaskManager& task_manager_;
};

class ListCommand : public Command<ListCommand> {
public:
    ListCommand(TaskManager& manager);
    void executeImpl(const std::string& args);

private:
    TaskManager& task_manager_;
};

class UpdateCommand : public Command<UpdateCommand> {
public:
    UpdateCommand(TaskManager& manager);
    void executeImpl(const std::string& args);

private:
    TaskManager& task_manager_;
};