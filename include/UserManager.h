#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <map>
#include <string>

class UserManager {
private:
    std::map<std::string, std::string> users;
    //map:键（用户名）是唯一的，确保了每个用户名在 map 中只能存在一次。
public:
    UserManager();

    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
    bool checkUser(const std::string& username, const std::string& password);
    bool userExists(const std::string& username);
    void saveUsers();
    void loadUsers();
    std::string hashPassword(const std::string& password);
};
#endif
