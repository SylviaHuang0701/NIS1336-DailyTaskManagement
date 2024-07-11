#include "TaskManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>

bool compareTasksByStartTime(const Task& a, const Task& b) {
    return a.startTime < b.startTime;
}

TaskManager::TaskManager(const std::string& username) : username(username) {}

bool TaskManager::addTask(const Task& task) {
    for (const auto& t : tasks) {
        if(t.startTime == task.startTime){
            std::cerr << "A task with the same start time already exists." << std::endl;
            return false;
        }
        if (t.name == task.name && t.startTime == task.startTime) {
            std::cerr << "Task with the same name and start time already exists!" << std::endl;
            return false;
        }
    }
    tasks.push_back(task);
    saveTasks();
    return true;
}

bool TaskManager::deleteTask(int taskId) {
    auto it = std::remove_if(tasks.begin(), tasks.end(), [taskId](const Task& task) { return task.id == taskId; });
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        saveTasks();
        return true;
    }
    std::cerr << "Task not found!" << std::endl;
    return false;
}
void TaskManager::showTasks() {
    std::cout << "All Tasks for User: " << username << std::endl;
    for (const auto& task : tasks) {
        task.display();
    }
}

void TaskManager::showTasks(std::string dateStr) {
    std::tm date = {};
    std::istringstream ss(dateStr);
    if (dateStr.length() == 7) { // YYYY-MM
        ss >> std::get_time(&date, "%Y-%m");
    } else if (dateStr.length() == 10) { // YYYY-MM-DD
        ss >> std::get_time(&date, "%Y-%m-%d");
    } else {
        std::cerr << "Invalid date format: " << dateStr << std::endl;
        return;
    }

    std::vector<Task> tasksToShow;

    for (const auto& task : tasks) {
        std::tm* tmStartTime = std::localtime(&task.startTime);

        int taskYear = tmStartTime->tm_year + 1900;
        int taskMonth = tmStartTime->tm_mon + 1;
        int taskDay = tmStartTime->tm_mday;

        if (taskYear == (date.tm_year + 1900) && taskMonth == (date.tm_mon + 1) &&
            (dateStr.length() == 10 ? taskDay == date.tm_mday : true)) {
            tasksToShow.push_back(task);
        }
    }

    std::sort(tasksToShow.begin(), tasksToShow.end(), compareTasksByStartTime);

    std::cout << "Tasks on " << dateStr << ":" << std::endl;
    for (const auto& task : tasksToShow) {
        task.display();
    }
}

void TaskManager::saveTasks() {
    std::string filePath = "../data/" + username + "_tasks.txt";
    std::ofstream file(filePath, std::ofstream::app); // 打开文件以追加模式写入

    const Task& task = tasks.back();

    // 写入最后一个任务到文件末尾
    file << task.id << "," << task.name << "," << task.startTime << ","
         << static_cast<int>(task.priority) << "," << static_cast<int>(task.category) << ","
         << task.reminderTime << std::endl;
}

void TaskManager::loadTasks() {
    std::string filePath = "../data/" + username + "_tasks.txt";
    std::ifstream file(filePath);
    file.seekg(0, std::ios::beg);  // 将文件指针设置到文件开头
    if (!file.is_open()) {
        std::cerr << "Failed to open tasks file for reading.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name, startTimeStr, priorityStr, categoryStr, reminderTimeStr;
        int id;
        Priority priority;
        Category category;
        time_t startTime, reminderTime;

        // 使用 getline 分别读取每个字段，按逗号分隔
        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, startTimeStr, ',') &&
            std::getline(ss, priorityStr, ',') &&
            std::getline(ss, categoryStr, ',') &&
            std::getline(ss, reminderTimeStr)) {

            // 转换字符串为对应类型
            id = std::stoi(idStr);  // 转换 ID 字段
            startTime = std::atoll(startTimeStr.c_str());
            priority = static_cast<Priority>(std::stoi(priorityStr));
            category = static_cast<Category>(std::stoi(categoryStr));
            reminderTime = std::atoll(reminderTimeStr.c_str());

            // 创建任务并添加到任务列表
            tasks.emplace_back(name, startTime, reminderTime, priority, category);
        }
    }


    file.close();
}


void TaskManager::checkReminders() {
    while (true) {
        time_t now = time(0);
        for (const auto& task : tasks) {
            if (task.reminderTime == now) {
                std::cout << "Reminder: Task \"" << task.name << "\" is due now!" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
}



// int main() {
//     std::string username = "testuser";
//     TaskManager manager(username);

//     // 加载现有任务
//     manager.loadTasks();
//     std::cout << "Loaded tasks:\n";
//     manager.showTasks();

//     // 添加新任务
//     time_t now = time(0);
//     Task task1("Task1", now + 3600, now + 3000, Priority::High, Category::Study);
//     Task task2("Task2", now + 7200, now + 6000, Priority::Medium, Category::Life);

//     std::cout << "\nAdding tasks...\n";
//     if (manager.addTask(task1)) {
//         std::cout << "Task1 added successfully.\n";
//     } else {
//         std::cerr << "Failed to add Task1.\n";
//     }


//     if (manager.addTask(task2)) {
//         std::cout << "Task2 added successfully.\n";
//     } else {
//         std::cerr << "Failed to add Task2.\n";
//     }

//     // 显示所有任务
//     std::cout << "\nCurrent tasks:\n";
//     manager.showTasks();

//     // 删除任务
//     std::cout << "\nDeleting Task1...\n";
//     if (manager.deleteTask(task1.id)) {
//         std::cout << "Task1 deleted successfully.\n";
//     } else {
//         std::cerr << "Failed to delete Task1.\n";
//     }

//     // 显示所有任务
//     std::cout << "\nCurrent tasks after deletion:\n";
//     manager.showTasks();

//     // 启动提醒检查（在后台线程中运行）
//     std::cout << "\nStarting reminder check...\n";
//     std::thread reminderThread(&TaskManager::checkReminders, &manager);
//     reminderThread.detach();

//     // 模拟主程序的长时间运行
//     std::this_thread::sleep_for(std::chrono::seconds(120)); // 运行2分钟

//     return 0;
// }

