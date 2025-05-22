#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <variant>

#include "command_wrapper.h"
#include "extra_command.h"
#include "logger.h"
#include "task_manager.h"

int main() {
    TaskManager task_manager;

    // 方式一: 虚基类
    // std::unordered_map<std::string,
    // std::unique_ptr<CommandBase>> commands;
    // commands["add"] =
    // std::make_unique<AddCommand>(task_manager);
    // commands["delete"] =
    // std::make_unique<DeleteCommand>(task_manager);
    // commands["list"] =
    // std::make_unique<ListCommand>(task_manager);
    // commands["update"] =
    // std::make_unique<UpdateCommand>(task_manager);

    // 方式二: 类型擦除
    // std::unordered_map<std::string, std::function<void(const std::string&)>>
    // commands; 伪闭包 使用shared_ptr来存储命令对象，由其来管理对象的生命周期
    // auto add_command = std::make_shared<AddCommand>(task_manager);
    // auto delete_command = std::make_shared<DeleteCommand>(task_manager);
    // auto list_command = std::make_shared<ListCommand>(task_manager);
    // auto update_command = std::make_shared<UpdateCommand>(task_manager);
    // commands["add"] = [add_command](const std::string& args) -> void {
    // add_command->execute(args); }; commands["delete"] =
    // [delete_command](const std::string& args)
    // -> void { delete_command->execute(args); }; commands["list"] =
    // [list_command](const std::string& args) -> void {
    // list_command->execute(args); }; commands["update"] =
    // [update_command](const std::string& args) -> void {
    // update_command->execute(args); };

    // 方式三: std::variant,C++17新特性
    // using CommandVariant = std::variant<
    //     std::unique_ptr<AddCommand>, std::unique_ptr<DeleteCommand>,
    //     std::unique_ptr<ListCommand>, std::unique_ptr<UpdateCommand> >;
    // std::unordered_map<std::string, CommandVariant> commands;
    // commands["add"] = std::make_unique<AddCommand>(task_manager);
    // commands["delete"] = std::make_unique<DeleteCommand>(task_manager);
    // commands["list"] = std::make_unique<ListCommand>(task_manager);
    // commands["update"] = std::make_unique<UpdateCommand>(task_manager);

    // 方式四: 封装类
    std::unordered_map<std::string, CommandWrapper> commands;
    commands.emplace("add", AddCommand(task_manager));
    commands.emplace("delete", DeleteCommand(task_manager));
    commands.emplace("list", ListCommand(task_manager));
    commands.emplace("update", UpdateCommand(task_manager));

    std::cout << "Welcome to the Task Manager!" << std::endl;
    std::cout << "Available commands: add, delete, list, update, exit"
              << std::endl;

    std::string input;
    while (true) {
        std::cout << "\n○ wenao@wenao:~$ ";
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }

        size_t space_pos = input.find(' ');
        std::string command = input.substr(0, space_pos);
        std::string args;
        if (space_pos != std::string::npos) {
            args = input.substr(space_pos + 1);
        }

        if (command == "exit ") {
            std::cout << "Exiting the Task Manager.\n" << std::endl;
            Logger::getInstance().log("Task Manager exited.\n");
            break;
        } else if (command == "exit") {
            std::cout << "Exiting the Task Manager.\n" << std::endl;
            Logger::getInstance().log("Task Manager exited.\n");
            break;
        }

        auto it = commands.find(command);
        if (it != commands.end()) {
            // std::visit([&args](auto&& cmd_ptr) { cmd_ptr->execute(args); },
            //            it->second);
            it->second.execute(args);
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}