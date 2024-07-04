#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <ctime>
#include <string>
#include <vector>

enum class Priority {
    High,
    Medium,
    Low,
    Unknown
};

enum class Category {
    Study,
    Entertainment,
    Life,
    Unknown
};

std::string priorityToString(Priority priority);
std::string categoryToString(Category category);

class Task {
private:
    static int nextId;
    int id;
    std::string name;
    time_t startTime;
    Priority priority;
    Category category;
    time_t reminderTime;

public:
    Task(const std::string& name, time_t startTime, const Priority& priority, const Category& category, time_t reminderTime);
    int getId() const { return id; }
    std::string getName() const { return name; }
    time_t getStartTime() const { return startTime; }
    Priority getPriority() const { return priority; }
    Category getCategory() const { return category; }
    time_t getReminderTime() const { return reminderTime; }
    void display() const;
};

#endif
