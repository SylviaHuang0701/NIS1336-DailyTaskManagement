#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include <map>
#include <string>

class UserManager {
private:
    std::map<std::string, std::string> userDatabase; // 用户名到哈希密码的映射

public:
    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
    void loadUsers();
    void saveUsers();
    std::string hashPassword(const std::string& password);
};

#endif
