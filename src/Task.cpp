#include "Task.h"
#include <iostream>

int Task::nextId = 1;

Task::Task(const std::string& name, time_t startTime, const Priority& priority, const Category& category, time_t reminderTime)
    : id(nextId++), name(name), startTime(startTime), priority(priority), category(category), reminderTime(reminderTime) {}

std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::High: return "High";
        case Priority::Medium: return "Medium";
        case Priority::Low: return "Low";
        default: return "Unknown";
    }
}

std::string categoryToString(Category category) {
    switch (category) {
        case Category::Study: return "Study";
        case Category::Entertainment: return "Entertainment";
        case Category::Life: return "Life";
        default: return "Unknown";
    }
}

void Task::display() const {
    std::cout << "ID: " << id << ", Name: " << name << ", Start Time: " << ctime(&startTime)
              << "Priority: " << priorityToString(priority) << ", Category: " << categoryToString(category)
              << ", Reminder Time: " << ctime(&reminderTime) << std::endl;
}
