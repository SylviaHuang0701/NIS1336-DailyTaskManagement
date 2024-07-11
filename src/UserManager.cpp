#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

UserManager::UserManager() {
    loadUsers();
}

bool UserManager::login(const std::string& username, const std::string& password) {
    //std::string hashedPassword = hashPassword(password);
    if (users.find(username) != users.end() && users[username] == password) {
        return true;
    }
    return false;
}
bool UserManager::userExists(const std::string& username) {
        return users.find(username) != users.end();
}

bool UserManager::checkUser(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it != users.end()) {
        if (it->second == password) {
            return true;  // Password is correct
        } else {
            std::cerr << "Incorrect password.\n";
            return false;  // Password is incorrect
        }
    }
    std::cerr << "User does not exist.\n";
    return false;  // User does not exist
}

bool UserManager::registerUser(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        return false;
    }
    users[username] = password;
    saveUsers();
    return true;
}

void UserManager::loadUsers() {
    std::ifstream file("../data/users.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password;
        std::getline(ss, username, ',');
        //getline(输入流，存储信息的位置，截断字符)
        //users.txt形如：
        /*
        Alice,fghujifghj
        Bob,txfcgvhbj
        */
        std::getline(ss, password, '\n');
        users[username] = password;
    }
}

void UserManager::saveUsers() {
    std::ofstream file("../data/users.txt");
    //基于范围的map遍历
    for (const auto& it : users) {
        file << it.first << "," << it.second << std::endl;
    }
}

std::string UserManager::hashPassword(const std::string& password) {

}

// int main(){
//     UserManager userManager;
//     userManager.loadUsers();

//     std::string username, password;
//     std::cout << "Enter username: ";
//     std::cin >> username;
//     std::cout << "Enter password: ";
//     std::cin >> password;

//     if (!userManager.login(username, password)) {
//         std::cout << "Invalid username or password.\n";
//         userManager.registerUser(username, password);
//     }else{
//         std::cout<<"success!"<<std::endl;
//     }

//     return 0;
// }