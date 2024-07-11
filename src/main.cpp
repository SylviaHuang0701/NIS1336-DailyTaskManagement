#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>
#include "UserManager.h"
#include "TaskManager.h"
#include "Task.h"

void startReminderThread(TaskManager& manager) {
    std::thread reminderThread([&manager]() {
        while (true) {
            manager.checkReminders();
            std::this_thread::sleep_for(std::chrono::minutes(1));
        }
    });
    reminderThread.detach();
}

time_t parseDateTime(const std::string& dateTime) {
    std::tm tm = {};
    std::istringstream ss(dateTime);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date time");
    }
    return mktime(&tm);
}

void printHelp() {
    std::cout << "Usage: myschedule [command]\n"
              << "Commands:\n"
              << "  run                : Start interactive mode\n"
              << "  help               : Show this help message\n";
}

void printInteractiveHelp() {
    std::cout << "Available commands in interactive mode:\n"
              << "  addtask [name] [start time] [priority] [category] [reminder time]\n"
              << "          Start time and reminder time format: YYYY-MM-DD HH:MM:SS\n"
              << "          Priority values: High (0), Medium (1), Low (2), Unknown (3)\n"
              << "          Category values: Study (0), Entertainment (1), Life (2), Unknown (3)\n"
              << "  deltask [task id]  : Delete a task by ID\n"
              << "  showtasks          : Show all tasks\n"
              << "  quit               : Exit the interactive mode\n";
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid number of arguments.\n";
        printHelp();
        return 1;
    }

    std::string command = argv[1];

    if (command == "help") {
        printHelp();
        return 0;
    }

    if (command == "run") {
        std::string username, password;
        UserManager userManager;
        TaskManager* manager = nullptr;

        while (true) {
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << "Password: ";
            std::getline(std::cin, password);

            if (userManager.userExists(username)) {
                if (userManager.checkUser(username, password)) {
                    manager = new TaskManager(username);
                    manager->loadTasks();
                    startReminderThread(*manager);
                    break;
                } else {
                    std::cerr << "Incorrect password. Please try again.\n";
                }
            } else {
                std::cerr << "User does not exist.\n";
                int flag;
                std::cout << "Do you want to register?(Yes:1/No:0): ";
                std::cin >> flag;
                std::cin.ignore();
                if (flag) {
                    if (userManager.registerUser(username, password)) {
                        std::cout << "User registered successfully.\n";
                        manager = new TaskManager(username);
                        manager->loadTasks();
                        startReminderThread(*manager);
                        break;
                    } else {
                        std::cerr << "Failed to register user.\n";
                    }
                } else {
                    return 1;
                }
            }
        }

        std::cout << "Welcome, " << username << "!\n";
        printInteractiveHelp();

        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            std::stringstream ss(input);
            std::string cmd;
            ss >> cmd;

            if (cmd == "addtask") {
                std::string name;
                std::string startTimeStr, reminderTimeStr;
                int priority, category;
                ss >> name >> std::quoted(startTimeStr) >> priority >> category >> std::quoted(reminderTimeStr);
                time_t startTime = parseDateTime(startTimeStr);
                time_t reminderTime = parseDateTime(reminderTimeStr);
                Task newTask(name, startTime, reminderTime, static_cast<Priority>(priority), static_cast<Category>(category));
                if (manager->addTask(newTask)) {
                    std::cout << "Task added successfully.\n";
                } else {
                    std::cerr << "Failed to add task.\n";
                }
            } else if (cmd == "deltask") {
                int taskId;
                ss >> taskId;
                if (manager->deleteTask(taskId)) {
                    std::cout << "Task deleted successfully.\n";
                } else {
                    std::cerr << "Failed to delete task.\n";
                }
            } else if (cmd == "showtasks") {
                std::string dateStr;
                ss >> dateStr;
                if (dateStr.empty()) {
                    manager->showTasks();
                } else {
                    manager->showTasks(dateStr);
                }
            } else if (cmd == "quit") {
                break;
            } else {
                std::cerr << "Unknown command.\n";
                printInteractiveHelp();
            }
        }

        delete manager;
    } else {
        std::cerr << "Unknown command.\n";
        printHelp();
    }

    return 0;
}



