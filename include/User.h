#ifndef USER_H
#define USER_H

#include "TaskManager.h"
#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string passwordHash;
    TaskManager taskManager;

public:
    User(const std::string& username, const std::string& passwordHash);
    std::string getUsername() const;
    std::string getPasswordHash() const;
    TaskManager& getTaskManager();
};

#endif
