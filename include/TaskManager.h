#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <string>
#include "Task.h"

class TaskManager {
private:
    std::string username;
    std::vector<Task> tasks;

    void saveTasks();

public:
    TaskManager(const std::string& username);
    bool addTask(const Task& task);
    bool deleteTask(int taskId);
    void showTasks();
    void showTasks(std::string dateStr);
    void loadTasks();
    void checkReminders();
};

#endif



