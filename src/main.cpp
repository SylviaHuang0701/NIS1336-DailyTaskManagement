#include <iostream>
#include "UserManager.h"
#include "TaskManager.h"
#include <thread>
#include <chrono>

void startReminderThread(TaskManager& manager) {
    std::thread reminderThread([&manager]() {
        while (true) {
            manager.checkReminders();
            std::this_thread::sleep_for(std::chrono::minutes(1));
        }
    });
    reminderThread.detach();
}

int main() {
    UserManager userManager;
    userManager.loadUsers();

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (!userManager.login(username, password)) {
        std::cerr << "Invalid username or password.\n";
        return 1;
    }

    User user(username, userManager.hashPassword(password));
    TaskManager& taskManager = user.getTaskManager();
    taskManager.loadTasks(username);

    startReminderThread(taskManager);

    // 提供命令行接口来管理任务
    // 添加、删除、显示任务等
    int choice;
    do {
        std::cout << "1. Add Task\n2. Delete Task\n3. Show Tasks\n4. Exit\nEnter choice: ";
        std::cin >> choice;
        if (choice == 1) {
            std::string name;
            time_t startTime, reminderTime;
            int priority, category;
            std::cout << "Enter task name: ";
            std::cin >> name;
            std::cout << "Enter start time (epoch): ";
            std::cin >> startTime;
            std::cout << "Enter priority (0-High, 1-Medium, 2-Low): ";
            std::cin >> priority;
            std::cout << "Enter category (0-Study, 1-Entertainment, 2-Life): ";
            std::cin >> category;
            std::cout << "Enter reminder time (epoch): ";
            std::cin >> reminderTime;

            taskManager.addTask(Task(name, startTime, static_cast<Priority>(priority), static_cast<Category>(category), reminderTime));
            taskManager.saveTasks(username);
        } else if (choice == 2) {
            int taskId;
            std::cout << "Enter task ID to delete: ";
            std::cin >> taskId;
            taskManager.deleteTask(taskId);
            taskManager.saveTasks(username);
        } else if (choice == 3) {
            taskManager.showTasks();
        }
    } while (choice != 4);

    userManager.saveUsers();

    return 0;
}

