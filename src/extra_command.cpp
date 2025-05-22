#include "extra_command.h"

AddCommand::AddCommand(TaskManager& manager) : task_manager_(manager) {}

void AddCommand::executeImpl(const std::string& args) {
    // Implementation of the add command
    size_t pos1 = args.find(',');
    size_t pos2 = args.find(',', pos1 + 1);
    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        Logger::getInstance().log("Invalid arguments for command");
        std::cout << "Usage: add <description>,<priority>,<due_date>"
                  << std::endl;
        return;
    }

    std::string description = args.substr(0, pos1);
    int priority = std::stoi(args.substr(pos1 + 1, pos2 - pos1 - 1));
    std::string due_date = args.substr(pos2 + 1);
    task_manager_.addTask(description, priority, due_date);
    std::cout << "Task added successfully." << std::endl;
}

DeleteCommand::DeleteCommand(TaskManager& manager) : task_manager_(manager) {}

void DeleteCommand::executeImpl(const std::string& args) {
    // Implementation of the delete command
    try {
        size_t pos;
        int id = std::stoi(args, &pos);
        if (pos != args.length()) {
            Logger::getInstance().log("Invalid arguments for command");
            std::cout << "Usage: delete <task_id>" << std::endl;
            return;
        }
        task_manager_.removeTask(id);
        std::cout << "Task deleted successfully." << std::endl;
    } catch (const std::invalid_argument& e) {
        Logger::getInstance().log("Error occurred while deleting task");
        std::cout << "Usage: delete <task_id>" << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        Logger::getInstance().log("Task ID out of range");
        return;
    }
}

ListCommand::ListCommand(TaskManager& manager) : task_manager_(manager) {}

void ListCommand::executeImpl(const std::string& args) {
    // Implementation of the list command
    try {
        int sort_option = 0;
        if (!args.empty()) {
            sort_option = std::stoi(args);
        }

        std::cout << "Current tasks: " << std::endl;
        task_manager_.listTasks(sort_option);
    } catch (const std::invalid_argument& e) {
        Logger::getInstance().log("Invalid arguments for command");
        return;
    } catch (const std::out_of_range& e) {
        Logger::getInstance().log("Sort option out of range");
        return;
    }
}

UpdateCommand::UpdateCommand(TaskManager& manager) : task_manager_(manager) {}

void UpdateCommand::executeImpl(const std::string& args) {
    // Implementation of the update command
    try {
        size_t pos1 = args.find(',');
        size_t pos2 = args.find(',', pos1 + 1);
        size_t pos3 = args.find(',', pos2 + 1);
        if (pos1 == std::string::npos || pos2 == std::string::npos ||
            pos3 == std::string::npos) {
            Logger::getInstance().log("Invalid arguments for command");
            std::cout
                << "Usage: update <task_id>,<description>,<priority>,<due_date>"
                << std::endl;
            return;
        }
        int id = std::stoi(args.substr(0, pos1));
        std::string description = args.substr(pos1 + 1, pos2 - pos1 - 1);
        int priority = std::stoi(args.substr(pos2 + 1, pos3 - pos2 - 1));
        std::string due_date = args.substr(pos3 + 1);
        task_manager_.updateTask(id, description, priority, due_date);
        std::cout << "Task updated successfully." << std::endl;
    } catch (const std::invalid_argument& e) {
        Logger::getInstance().log("Error occurred while updating task");
        return;
    } catch (const std::out_of_range& e) {
        Logger::getInstance().log("Task ID out of range");
        return;
    }
}
