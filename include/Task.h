#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>

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
public:
    static int nextId;
    int id;
    std::string name;
    time_t startTime;
    Priority priority;
    Category category;
    time_t reminderTime;

    Task(const std::string& name, time_t startTime, time_t reminderTime, const Priority& priority = Priority::High, const Category& category = Category::Study);
    void display() const;
};

#endif


