#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "Task.h"
#include <vector>
#include <fstream>

class TaskManager {
private:
    std::vector<Task> tasks;
public:
    bool addTask(const Task& task);
    bool deleteTask(int taskId);
    void showTasks(const std::string& date);
    void saveTasks(const std::string& username);
    void loadTasks(const std::string& username);
    void checkReminders();
};

#endif


bool TaskManager::addTask(const Task& task) {
    // 实现任务录入逻辑
}

bool TaskManager::deleteTask(int taskId) {
    // 实现任务删除逻辑
}

void TaskManager::showTasks(const std::string& date) {
    // 实现任务显示逻辑
}

void TaskManager::loadTasks(const std::string& username) {
    // 从文件加载任务
}

void TaskManager::saveTasks(const std::string& username) {
    // 将任务保存到文件
}

void TaskManager::checkReminders() {
    // 检查任务提醒
}

