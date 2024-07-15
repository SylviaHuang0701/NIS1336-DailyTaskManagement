# 任务管理系统设计文档

## 小组成员
黄兰婷: 结构体与类的设计(Task TaskManager UserManager)，main函数编写，多线程编写,流程图绘制，部分GUI适配

刘佳琪: 所有GUI改编，流程图绘制

## 模块与类设计

### 模块

1. `bin`: 存放可执行文件。
2. `build`: 存放构建过程中生成的文件和中间文件。
3. `data`: 存放用户任务数据和用户信息文件。
4. `include`: 存放头文件。
5. `src`: 存放源代码文件。

### 类设计

#### `Task`

负责表示单个任务的信息。

- **成员变量**:
  - `id`: 任务ID
  - `name`: 任务名称
  - `startTime`: 任务开始时间，以time_t形式存储
  - `reminderTime`: 任务提醒时间，以time_t形式存储
  - `priority`: 任务优先级
  - `category`: 任务类别

- **成员函数**:
  - 构造函数
  - `display()`: 显示任务信息

#### `TaskManager`

负责管理用户的任务。

- **成员变量**:
  - `username`: 用户名
  - `tasks`: 存储任务的列表

- **成员函数**:
  - 构造函数
  - `addTask(const Task&)`: 添加任务
  - `deleteTask(int)`: 删除任务
  - `showTasks()`: 显示所有任务
  - `showTasks(std::string)`: 显示指定日期或月份的任务
  - `saveTasks()`: 保存任务到文件
  - `loadTasks()`: 从文件加载任务
  - `checkReminders()`: 检查任务提醒

#### `UserManager`

负责管理用户信息。

- **成员变量**:
  - `users`: 存储用户信息的列表

- **成员函数**:
  - 构造函数
  - `userExists(const std::string&)`: 检查用户是否存在
  - `login(const std::string&, const std::string&)`: 用户登录
  - `registerUser(const std::string&, const std::string&)`: 注册用户

## 具体函数代码
### 函数：`loadUsers()`

- **函数功能：** 加载已注册用户的数据并存储在内存中的`users` map中。从存储用户数据的文件`../data/users.txt`中读取用户名和密码，并将它们存储为键值对。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void UserManager::loadUsers() {
    std::ifstream file("../data/users.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password;
        std::getline(ss, username, ',');
        std::getline(ss, password, '\n');
        users[username] = password;
    }
}
```

#### 说明

`loadUsers()` 函数通过打开存储用户信息的文件，逐行读取数据，并将每行的用户名和密码分别存储在 `users` map 中的键值对中。文件中每行的格式为 `username,password`，其中 `username` 是用户的用户名，`password` 是经过哈希处理的密码字符串。

---

### 函数：`saveUsers()`

- **函数功能：** 将当前内存中的用户数据保存到文件中。将`users` map 中的每个用户名和密码组成一行文本，并写入到文件`../data/users.txt`中。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void UserManager::saveUsers() {
    std::ofstream file("../data/users.txt");
    for (const auto& it : users) {
        file << it.first << "," << it.second << std::endl;
    }
}
```

#### 说明

`saveUsers()` 函数遍历存储用户信息的 `users` map，将每个用户名和对应的密码按照 `username,password` 的格式写入到文件中。每个用户信息占据文件中的一行，以便后续能够重新加载和验证用户数据。

---

### 函数：`userExists(const std::string& username)`

- **函数功能：** 检查给定用户名是否已存在于内存中的 `users` map 中。
- **函数输入：** `username` - 待检查的用户名
- **函数输出：** `true` - 用户名已存在；`false` - 用户名不存在

#### 函数实现

```cpp
bool UserManager::userExists(const std::string& username) {
    return users.find(username) != users.end();
}
```

#### 说明

`userExists()` 函数通过查找 `users` map 中是否存在给定的 `username` 来判断用户是否已经注册。如果 `username` 存在于 `users` map 中，则返回 `true`，否则返回 `false`。

---

### 函数：`registerUser(const std::string& username, const std::string& password)`

- **函数功能：** 注册新用户，将用户名和经过哈希处理的密码存储在内存中的 `users` map 中，并将用户数据保存到文件中。
- **函数输入：** `username` - 新用户的用户名；`password` - 新用户的密码
- **函数输出：** `true` - 注册成功；`false` - 用户名已存在，注册失败

#### 函数实现

```cpp
bool UserManager::registerUser(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        return false; // 用户已存在，注册失败
    }
    std::string hashedPassword = hashPassword(password);
    users[username] = hashedPassword;
    saveUsers();
    return true; // 注册成功
}
```

#### 说明

`registerUser()` 函数首先检查是否已经存在相同的 `username`，如果存在则注册失败并返回 `false`；如果不存在，则对密码进行哈希处理后，将用户名和哈希后的密码存储到 `users` map 中，并调用 `saveUsers()` 将新的用户数据持久化到文件中。注册成功后返回 `true`。

---

### 函数：`login(const std::string& username, const std::string& password)`

- **函数功能：** 用户登录验证，检查给定的用户名和密码是否匹配。
- **函数输入：** `username` - 用户名；`password` - 密码
- **函数输出：** `true` - 登录成功；`false` - 登录失败

#### 函数实现

```cpp
bool UserManager::login(const std::string& username, const std::string& password) {
    std::string hashedPassword = hashPassword(password);
    if (users[username] == hashedPassword) {
        return true; // 登录成功
    }
    return false; // 登录失败
}
```

#### 说明

`login()` 函数首先将用户输入的密码进行哈希处理，然后将处理后的哈希密码与内存中存储的对应用户名的哈希密码进行比较。如果匹配，则登录成功并返回 `true`，否则返回 `false` 表示登录失败。

---

### 函数：`hashPassword(const std::string& password)`

- **函数功能：** 使用 MD5 算法对输入的密码进行哈希处理，返回哈希后的字符串。
- **函数输入：** `password` - 待哈希处理的密码
- **函数输出：** 哈希处理后的密码字符串

#### 函数实现

```cpp
std::string UserManager::hashPassword(const std::string& password) {
    return MD5(password).toStr(); // 使用 MD5 算法进行密码哈希处理
}
```

#### 说明

`hashPassword()` 函数通过调用 `MD5` 类对输入的密码进行 MD5 哈希处理，并返回处理后的哈希字符串。这样可以确保用户密码在存储和传输过程中更安全。

---
### 函数：`addTask(const Task& task)`

- **函数功能：** 向用户的任务列表中添加新任务，并将任务数据保存到文件中。
- **函数输入：** `task` - 要添加的任务对象
- **函数输出：** `true` - 添加成功；`false` - 添加失败（任务已存在或其他错误）

#### 函数实现

```cpp
bool TaskManager::addTask(const Task& task) {
    for (const auto& t : tasks) {
        if (t.startTime == task.startTime) {
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
```

#### 说明

`addTask()` 函数首先检查要添加的任务是否与现有任务具有相同的开始时间或相同的名称和开始时间。如果存在重复，函数将打印错误消息并返回 `false`。如果不存在重复，将任务添加到 `tasks` 容器中，并调用 `saveTasks()` 函数将更新后的任务列表保存到文件中。添加成功后返回 `true`。

---

### 函数：`deleteTask(int taskId)`

- **函数功能：** 根据任务 ID 删除用户的特定任务，并将更新后的任务列表保存到文件中。
- **函数输入：** `taskId` - 要删除的任务的 ID
- **函数输出：** `true` - 删除成功；`false` - 删除失败（未找到指定 ID 的任务）

#### 函数实现

```cpp
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
```

#### 说明

`deleteTask()` 函数通过使用 Lambda 表达式查找并删除 `tasks` 容器中具有指定 `taskId` 的任务。如果找到任务并成功删除，则调用 `saveTasks()` 函数将更新后的任务列表保存到文件中，并返回 `true`。如果未找到指定 ID 的任务，则打印错误消息并返回 `false`。

---

### 函数：`showTasks()`

- **函数功能：** 显示用户所有的任务信息。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void TaskManager::showTasks() {
    std::cout << "All Tasks for User: " << username << std::endl;
    for (const auto& task : tasks) {
        task.display();
    }
}
```

#### 说明

`showTasks()` 函数遍历并显示存储在 `tasks` 容器中的所有任务信息。函数首先打印当前用户的用户名，然后依次调用每个任务对象的 `display()` 函数显示任务的详细信息。

---

### 函数：`showTasks(std::string dateStr)`

- **函数功能：** 根据指定的日期字符串（YYYY-MM 或 YYYY-MM-DD）显示用户在该日期上的所有任务。
- **函数输入：** `dateStr` - 指定的日期字符串
- **函数输出：** 无

#### 函数实现

```cpp
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
```

#### 说明

`showTasks(std::string dateStr)` 函数根据输入的日期字符串 `dateStr`（可以是 `YYYY-MM` 或 `YYYY-MM-DD` 格式）筛选并显示用户在该日期上的所有任务。函数首先解析日期字符串并将其转换为 `tm` 结构体表示的日期对象，然后遍历用户所有的任务，将符合日期条件的任务存储在 `tasksToShow` 容器中。最后，对 `tasksToShow` 中的任务按照开始时间排序，并依次调用任务对象的 `display()` 函数显示任务的详细信息。

---

### 函数：`saveTasks()`

- **函数功能：** 将当前内存中的任务列表保存到用户特定的任务文件中。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void TaskManager::saveTasks() {
    std::string filePath = "../data/" + username + "_tasks.txt";
    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    for (const auto& task : tasks) {
        file << task.id << "," << task.name << "," << task.startTime << ","
             << static_cast<int>(task.priority) << "," << static_cast<int>(task.category) << ","
             << task.reminderTime << std::endl;
    }

    file.close();
}
```

#### 说明

`saveTasks()` 函数创建或覆盖用户特定的任务文件（文件路径格式为 `../data/{username}_tasks.txt`），并将当前内存中存储的所有任务数据写入到文件中。函数遍历 `tasks` 容器中的每个任务对象，并将任务的 ID、名称、开始时间、优先级、类别和提醒时间写入文件中。保存完成后关闭文件流。

---

### 函数：`loadTasks()`

- **函数功能：** 加载用户特定的任务文件，并将文件中的任务数据读取到内存中的 `tasks` 容器中。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void TaskManager::loadTasks() {
    std::string filePath = "../data/" + username + "_tasks.txt";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open tasks file for reading." << std::endl;
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

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, startTimeStr, ',') &&
            std::getline(ss, priorityStr, ',') &&
            std::getline(ss, categoryStr, ',') &&
            std::getline(ss, reminderTimeStr)) {

            id = std::stoi(idStr);
            startTime = atoll(startTimeStr.c_str());
            priority = static_cast<Priority>(std::stoi(priorityStr));
            category = static_cast<Category>(std::stoi(categoryStr));
            reminderTime = atoll(reminderTimeStr.c_str());

            tasks.emplace_back(name, startTime, reminderTime, priority, category);
        }
    }

    file.close();
}
```

#### 说明

`loadTasks()` 函数打开用户特定的任务文件（文件路径格式为 `../data/{username}_tasks.txt`），并逐行读取文件中的任务数据。函数解析每行数据，并根据字段创建 `Task` 对象，并将其添加到 `tasks` 容

器中。加载完成后关闭文件流，并将所有读取的任务数据存储在内存中的 `tasks` 容器中供后续操作使用。

---

### 函数：`checkReminders()`

- **函数功能：** 检查用户任务列表中是否有提醒时间与当前时间相符的任务，并进行相应的提醒操作。
- **函数输入：** 无
- **函数输出：** 无

#### 函数实现

```cpp
void TaskManager::checkReminders() {
    time_t now = time(0);
    for (const auto& task : tasks) {
        if (task.reminderTime == now) {
            std::cout << "\033[1;31m"
                      << "Reminder: Task \"" << task.name << "\" is due now!"
                      << "\033[0m"
                      << std::endl;

            std::system("aplay ../notification.wav");

            std::system(("notify-send 'Task Reminder' 'Task \"" + task.name + "\" is due now!'").c_str());
        }
    }
}
```

#### 说明

`checkReminders()` 函数首先获取当前时间 `now`，然后遍历用户任务列表中的每个任务。对于每个任务，如果其提醒时间 `reminderTime` 等于当前时间 `now`，则输出提醒消息到控制台，并通过系统调用播放声音和发送桌面通知提醒用户当前任务即将到期或已到期。

---

### 函数：`reminderThreadFunc(void* arg)`

- **函数功能：** 提醒线程的执行函数，周期性地检查任务的提醒时间，并进行相应的提醒操作。
- **函数输入：** `arg` - 指向 `TaskManager` 对象的指针，用于执行任务管理操作
- **函数输出：** 无

#### 函数实现

```cpp
void* reminderThreadFunc(void* arg) {
    TaskManager* manager = static_cast<TaskManager*>(arg);
    while (!stopped) {
        pthread_mutex_lock(&mutex); // 加锁
        manager->checkReminders();
        pthread_mutex_unlock(&mutex); // 解锁

        sleep(1); // 每1s检查一次
    }
    return nullptr;
}
```

#### 说明

`reminderThreadFunc(void* arg)` 函数作为提醒线程的执行函数，通过循环在每次迭代中使用互斥锁保护，调用 `TaskManager` 对象的 `checkReminders()` 函数检查所有任务的提醒时间。如果某个任务的提醒时间与当前时间相符，将会通过命令行消息、声音和桌面通知进行提醒。

---

### 函数：`inputThreadFunc(void* arg)`

- **函数功能：** 输入处理线程的执行函数，处理用户在交互模式下输入的命令并执行相应的任务管理操作。
- **函数输入：** `arg` - 指向 `TaskManager` 对象的指针，用于执行任务管理操作
- **函数输出：** 无

#### 函数实现

```cpp
void* inputThreadFunc(void* arg) {
    TaskManager* manager = static_cast<TaskManager*>(arg);
    std::string input;
    while (!stopped) {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        std::string cmd;
        ss >> cmd;

        if (cmd == "addtask") {
            try {
                // 解析命令并添加任务的逻辑
                // ...
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << "\n";
            } catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        } else if (cmd == "deltask") {
            // 处理删除任务的逻辑
            // ...
        } else if (cmd == "showtasks") {
            // 处理显示任务的逻辑
            // ...
        } else if (cmd == "quit") {
            stopped = true;
            break;
        } else {
            std::cerr << "Unknown command.\n";
            printInteractiveHelp();
        }
    }
    return nullptr;
}
```

#### 说明

`inputThreadFunc(void* arg)` 函数作为输入处理线程的执行函数，通过循环等待用户在交互模式下输入命令，并根据命令类型调用 `TaskManager` 对象的相应方法来处理任务的添加、删除、显示及退出等操作。对于每个命令，函数会根据输入进行适当的错误处理和用户提示。


### 函数：`main(int argc, char* argv[])`

- **函数功能：** 主函数，负责解析命令行参数，选择并执行对应的任务管理命令或显示帮助信息。
- **函数输入：** `argc` - 命令行参数个数，`argv` - 命令行参数数组
- **函数输出：** 返回整数值，表示程序执行状态（成功或失败）

#### 函数实现

```cpp
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid number of arguments.\n";
        printHelp();
        return 1;
    }

    std::string command = argv[1];

    if (command == "help") {
        printHelp();
        return 0;
    }

    if (command == "run") {
        // 交互模式逻辑
        // ...
    } else {
        std::cerr << "Unknown command.\n";
        printHelp();
    }

    return 0;
}
```
## GUI改编

### 类：`Login`

`Login` 类基于 Qt 框架实现，用于用户登录和注册管理，提供了简单的界面设计和用户交互功能。

#### 头文件：`login.h`

```cpp
#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(); // 构造函数，创建登录界面
    QString username_q;
    std::string username_s;

private:
    void createMenu(); // 创建菜单
    void createHorizontalGroupBox(); // 创建水平组框
    void createGridGroupBox(); // 创建网格组框
    void createFormGroupBox(); // 创建表单组框
    void onAccepted(); // 登录按钮响应函数
    void ifRegister(bool status); // 注册状态响应函数

    enum { NumGridRows = 3, NumButtons = 4 };
    QLineEdit *user_id = new QLineEdit; // 用户名输入框
    QLineEdit *password = new QLineEdit; // 密码输入框

    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox;
    QGroupBox *gridGroupBox;
    QGroupBox *formGroupBox;
    QTextEdit *smallEditor;
    QTextEdit *bigEditor;
    QLabel *labels[NumGridRows];
    QLineEdit *lineEdits[NumGridRows];
    QPushButton *buttons[NumButtons];
    QDialogButtonBox *buttonBox;

    QMenu *fileMenu;
    QAction *exitAction;

    bool flag = false, flag1 = true;

    QString password_q;
    std::string password_s;
};

#endif // LOGIN_H
```

#### 实现文件：`login.cpp`

```cpp
#include <QPushButton>
#include <QtWidgets>
#include "login.h"
#include "register.h"
#include "menu.h"
#include <string>
#include <QDebug>
#include <QString>
#include "UserManager.h"
#include "TaskManager.h"

Login::Login() {
    // 构造登录界面的布局和控件
    formGroupBox = new QGroupBox(tr("Sign In"));
    QFormLayout *layout = new QFormLayout;

    layout->addRow(new QLabel(tr("User Name:")), user_id);
    layout->addRow(new QLabel(tr("Password:")), password);
    formGroupBox->setLayout(layout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &Login::onAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setText(tr("sign in"));
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(buttonBox);

    QWidget *scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this); // 设置对话框的布局
    dialogLayout->addWidget(scrollArea);

    setWindowTitle(tr("Time Management Program"));
    setFixedSize(300, 150);
}

void Login::onAccepted() {
    // 登录按钮点击响应函数，验证用户信息并处理登录逻辑
    username_q = user_id->text();
    password_q = password->text();
    username_s = username_q.toStdString();
    password_s = password_q.toStdString();
    UserManager userManager;
    TaskManager* manager = nullptr;
    if (userManager.userExists(username_s)) {
        if (userManager.login(username_s, password_s)) {
            manager = new TaskManager(username_s);
            manager->loadTasks();
        } else {
            QMessageBox::information(nullptr, "Notice", "Incorrect password. Please try again.\n");
        }
    } else {
        Register *r = new Register(this);
        connect(r, &Register::do_register, this, &Login::ifRegister);
        r->exec();
        while(flag1){}
        if (flag) {
            if (userManager.registerUser(username_s, password_s)) {
                QMessageBox::information(nullptr, "Notice", "User registered successfully.");
                Menu *m = new Menu;
                manager = new TaskManager(username_s);
                manager->loadTasks();
            } else {
                QMessageBox::information(nullptr, "Notice", "Failed to register user.");
            }
        }
    }
    accept();
}

void Login::ifRegister(bool status) {
    // 注册状态响应函数，更新注册状态
    Login::flag = status;
    flag1 = false;
}

void Login::createFormGroupBox() {
    // 创建登录表单组框
    formGroupBox = new QGroupBox(tr("Sign In"));
    QFormLayout *layout = new QFormLayout;
    layout->addRow(new QLabel(tr("User Name:")), new QLineEdit);
    layout->addRow(new QLabel(tr("Password:")), new QLineEdit);
    formGroupBox->setLayout(layout);
}
```

#### 类功能

`Login` 类通过继承 `QDialog` 实现了一个简单的登录界面，包括输入用户名、密码、登录和注册功能。界面使用了多个 `QGroupBox` 和布局管理器来组织和显示控件。用户可以输入用户名和密码进行登录，或者点击注册按钮进行新用户注册，注册成功后显示成功提示信息。

---
### 类：`Register`
`Register` 类是一个基于 Qt 框架的注册界面，用于用户注册流程的实现。它提供了一个简单的界面，允许用户在用户不存在时选择是否进行注册。
#### 头文件：`register.h`
``` cpp
#ifndef REGISTER_H
#define REGISTER_H
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QtWidgets>
class Register : public QDialog
{
    Q_OBJECT
public:
    explicit Register(QWidget *parent = nullptr); // 构造函数，创建注册界面
private slots:
    void ifAccepted(); // 接受注册的槽函数
    void ifRejected(); // 拒绝注册的槽函数
signals:
    void do_register(bool &flag); // 发出一个注册状态的信号
private:
    QTextEdit *bigEditor; // 用于显示注册信息的文本编辑框
    QDialogButtonBox *buttonBox; // 按钮框，包含接受和拒绝按钮
    bool flag; // 用于存储用户是否选择注册的标志
};
#endif // REGISTER_H
```

#### 实现文件：`register.cpp`

```cpp
#include <QDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include "register.h"

// 构造函数，初始化注册界面
Register::Register(QWidget *parent) : QDialog(parent), flag(false)
{
    // 创建表单布局
    QFormLayout *layout = new QFormLayout;

    // 初始化文本编辑框，显示注册信息
    bigEditor = new QTextEdit;
    bigEditor->setPlainText(tr("User does not exist. Do you want to register?"));

    // 创建按钮框，包含"是"和"否"两个按钮
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &Register::ifAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &Register::ifRejected);

    // 设置按钮文本
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setText(tr("Yes"));
    }
    QPushButton *noButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (noButton) {
        noButton->setText(tr("No"));
    }

    // 将组件添加到布局中
    layout->addRow(bigEditor);
    layout->addWidget(buttonBox);

    // 设置窗口标题和固定大小
    setWindowTitle(tr("Time Management Program"));
    setFixedSize(300, 100);

    // 设置布局
    setLayout(layout);
}

// 槽函数，当用户选择注册时调用
void Register::ifAccepted()
{
    flag = true; // 设置标志为真，表示用户选择注册
    emit do_register(flag); // 发出注册状态信号
    close(); // 关闭注册对话框
}

// 槽函数，当用户选择不注册时调用
void Register::ifRejected()
{
    flag = false; // 设置标志为假，表示用户选择不注册
    emit do_register(flag); // 发出注册状态信号
    close(); // 关闭注册对话框
}
```
#### 类功能
`Register` 类实现了一个简单的注册对话框，提供了用户选择是否注册的选项。当用户选择注册时，它会发出一个信号，通知其他组件用户的选择。这个类通过继承 `QDialog` 来实现，使用了 `QTextEdit` 来显示注册信息，以及 `QDialogButtonBox` 来提供用户交互的按钮。通过 `ifAccepted` 和 ifRejected 槽函数来处理用户的响应，并设置内部标志位 `flag` 来表示用户的决定。

### 类：Menu
`Menu` 类是该用户时间管理程序中的一个核心组件，提供了一个用户界面，允许用户通过各种选项来管理任务。它集成了添加任务、删除任务、显示特定日期的任务列表以及接收提醒的功能。

#### 头文件：menu.h
```cpp
#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QFormLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include "addtask.h"
#include "deltask.h"
#include "TaskManager.h"
#include "TasksDialog.h"

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr); // 构造函数，创建菜单界面

    // 更新任务列表显示的方法
    void updateTaskList();
    // TaskManager 实例，用于任务管理
    TaskManager taskManager;

private slots:
    // 私有槽函数，用于添加任务
    void addTask();
    // 私有槽函数，用于删除任务
    void deleteTask();
    // 私有槽函数，用于按日期显示任务
    void showTasksForDate();
    // 处理从 TaskManager 接收到的任务列表
    void handleTasksReady(const std::vector<Task>& tasksToShow);
    // 显示提醒信息
    void displayReminder(const QString& message);

private:
    // 用于显示任务列表的文本编辑框
    QPlainTextEdit *plainTextEdit;
    // 包含操作按钮的按钮框
    QDialogButtonBox *buttonBox;
    // 用于触发显示特定日期任务的按钮
    QPushButton* showTasksButton;
    // 用户输入日期的行编辑框
    QLineEdit *dateLineEdit;
};

#endif // MENU_H
```
#### 实现文件：menu.cpp
```cpp
#include <QApplication>
#include <QFormLayout>
#include <QMessageBox>
#include <QDateTime>
#include <sstream>
#include <regex>
#include "menu.h"
#include "login.h"
#include "addtask.h"
#include "deltask.h"
#include "TaskManager.h"
#include "tasksDialog.h"

Menu::Menu(QWidget *parent) : QWidget(parent), taskManager(Login::username_s)
{
    // 初始化 plainTextEdit 控件并设置为只读
    QFormLayout *layout = new QFormLayout;
    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setReadOnly(true);

    // 初始化按钮框，并设置添加任务和删除任务按钮
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *addTaskButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *deleteTaskButton = buttonBox->button(QDialogButtonBox::Cancel);
    addTaskButton->setText(tr("Add new task"));
    deleteTaskButton->setText(tr("Delete task"));

    // 初始化日期输入和显示任务按钮
    QLabel *dateLabel = new QLabel(tr("Enter date :"));
    dateLineEdit = new QLineEdit(this);
    dateLineEdit->setPlaceholderText("e.g. 2024-07 / 2024-07-15");
    showTasksButton = new QPushButton(tr("Show Tasks"), this);

    // 将组件添加到布局中并设置信号和槽的连接
    layout->addRow(dateLabel, dateLineEdit);
    layout->addWidget(showTasksButton);
    connect(addTaskButton, &QPushButton::clicked, this, &Menu::addTask);
    connect(deleteTaskButton, &QPushButton::clicked, this, &Menu::deleteTask);
    connect(&taskManager, &TaskManager::tasksReady, this, &Menu::handleTasksReady);
    connect(&taskManager, &TaskManager::reminder, this, &Menu::displayReminder);
    connect(showTasksButton, &QPushButton::clicked, this, &Menu::showTasksForDate);

    // 初始化定时器，用于周期性检查提醒
    QTimer *reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, &taskManager, &TaskManager::checkReminders);
    reminderTimer->start(1000); // 每秒检查一次提醒

    // 首次显示时更新任务列表
    updateTaskList();

    // 将布局应用于 Menu 窗口，并设置窗口标题和大小
    layout->addWidget(plainTextEdit);
    layout->addWidget(buttonBox);
    setLayout(layout);
    setWindowTitle(tr("Time Management Program: Welcome!"));
    setFixedSize(600, 400);
}
```
#### 类功能
`Menu` 类提供了一个用户界面，允许用户执行以下操作：
1. 添加新任务：通过 `addTask` 槽函数激活 `AddTask` 对话框，用户可以输入任务的详细信息。
2. 删除任务：通过 `deleteTask` 槽函数激活 `Deltask` 对话框，用户可以通过输入任务 ID 来删除任务。
3. 按日期筛选任务：用户输入日期后，可以通过 `showTasksForDate` 槽函数显示该日期的任务列表。
4. 接收提醒：通过 `displayReminder` 槽函数，用户可以接收到任务提醒的弹窗通知。
5. 任务列表更新：`updateTaskList` 函数用于从 `TaskManager` 加载并显示当前的任务列表。
6. 定时:每秒触发一次 `TaskManager` 的 `checkReminders` 函数，以检查是否有任务提醒需要显示给用户。

### 类：AddTask
`AddTask` 类提供了一个基于 Qt 框架的图形用户界面（GUI），用于向任务管理程序中添加新任务。它允许用户输入任务的名称、开始时间、提醒时间、优先级和类型，并提供了必要的输入验证。

#### 头文件：`addtask.h`
```cpp
#ifndef ADDTASK_H
#define ADDTASK_H

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFormLayout>
#include "TaskManager.h"

class AddTask : public QDialog {
    Q_OBJECT

public:
    explicit AddTask(TaskManager& taskManager, QWidget *parent = nullptr); // 构造函数

signals:
    void addTaskSuccessful(); // 任务添加成功信号

private slots:
    void ifAccepted(); // 当用户点击"确定"按钮时调用的槽函数

private:
    TaskManager& taskManager; // 引用 TaskManager 实例，用于添加任务
    QLineEdit *lineEdits[5]; // 用于输入任务信息的 QLineEdit 数组
    QDialogButtonBox *buttonBox; // 包含"确定"和"取消"按钮的按钮框
};
#endif // ADDTASK_H
```
#### 实现文件：`addtask.cpp`
```cpp
#include <string>
#include <QDebug>
#include <QString>
#include "addtask.h"
#include "TaskManager.h"
#include "login.h"

// 将字符串形式的日期时间转换为 time_t 类型的时间戳
time_t parseDateTime(const std::string& dateTime) {
    std::tm tm = {};
    std::istringstream ss(dateTime);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date time");
    }
    return mktime(&tm);
}

AddTask::AddTask(TaskManager& taskManager, QWidget *parent) 
    : QDialog(parent), taskManager(taskManager) {
    // 初始化界面布局和组件
    QFormLayout *layout = new QFormLayout;

    // 创建并配置 QLineEdit 控件用于输入任务信息
    for (int i = 0; i < 5; ++i) {
        lineEdits[i] = new QLineEdit;
        layout->addRow(new QLabel(tr("Task name: ")), lineEdits[0]);
        // ... 为其他 lineEdits 设置标签和占位文本
    }

    // 创建并配置按钮框
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddTask::ifAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);

    // 设置对话框标题和大小
    setWindowTitle(tr("Time Management Program -- Add Task"));
    setFixedSize(500, 400);

    setLayout(layout);
}

void AddTask::ifAccepted() {
    // 处理用户点击"确定"后的逻辑
    QString temp;
    std::string task_name, start_time, reminder_time, priority_str, type_str;
    bool flag = true;

    // 获取输入并进行验证
    task_name = lineEdits[0]->text().toStdString();
    if (task_name.empty()) {
        QMessageBox::information(nullptr, "Notice", "Task name is required.");
        flag = false;
    }
    // ... 对其他输入进行验证

    if (flag) {
        try {
            // 将日期时间字符串转换为时间戳
            time_t startTime = parseDateTime(start_time);
            time_t reminderTime = parseDateTime(reminder_time);
            // 创建新任务并尝试添加到 TaskManager
            Task newTask(task_name, startTime, reminderTime, static_cast<Priority>(priority), static_cast<Category>(type));
            if (taskManager.addTask(newTask)) {
                QMessageBox::information(nullptr, "Notice", "Task added successfully.");
                emit addTaskSuccessful(); // 发出任务添加成功信号
                accept(); // 关闭对话框
            } else {
                QMessageBox::information(nullptr, "Notice", "Failed to add task.");
            }
        } catch (const std::exception& e) {
            QMessageBox::critical(nullptr, "Error", QString::fromStdString(e.what()));
        }
    }
}
```
#### 类功能
`AddTask` 类的主要功能是提供一个用户界面，允许用户通过填写表单来创建新的任务。用户可以指定任务的名称、开始时间、提醒时间、优先级和类型。输入验证确保了用户必须填写所有必要的信息，并且时间字段符合预期的格式。如果任务成功添加到 `TaskManager`，将发出 `addTaskSuccessful` 信号，提示用户任务添加成功，并关闭对话框。如果添加任务失败，将显示错误信息。

### 类：`Deltask`

`Deltask` 类提供了一个基于 Qt 框架的对话框，用于从任务管理程序中删除任务。用户可以通过输入任务的 ID 来指定要删除的任务。

#### 头文件：`deltask.h`

```cpp
#ifndef DELTASK_H
#define DELTASK_H

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFormLayout>
#include "TaskManager.h"

class Deltask : public QDialog {
    Q_OBJECT

public:
    // 构造函数，接受一个 TaskManager 的引用和父 QWidget 的指针
    explicit Deltask(TaskManager& taskManager, QWidget *parent = nullptr);

signals:
    // 任务删除成功的信号
    void taskDeleted();

private slots:
    // 槽函数，当用户点击"确定"按钮时调用
    void ifAccepted();

private:
    // TaskManager 的引用，用于访问任务管理功能
    TaskManager& taskManager;
    // 用于输入要删除的任务 ID 的 QLineEdit 控件
    QLineEdit *lineEdit;
    // 包含"确定"和"取消"按钮的 QDialogButtonBox 控件
    QDialogButtonBox *buttonBox;
};

#endif // DELTASK_H
```

#### 实现文件：`deltask.cpp`

```cpp
#include "deltask.h"
#include <QMessageBox>
#include <QString>

// 构造函数，初始化 Deltask 对象
Deltask::Deltask(TaskManager& taskManager, QWidget *parent)
    : QDialog(parent), taskManager(taskManager) {
    // 创建表单布局并添加任务 ID 输入框
    QFormLayout *layout = new QFormLayout;
    lineEdit = new QLineEdit;
    layout->addRow(new QLabel(tr("ID of the task you want to delete:")), lineEdit);

    // 创建按钮框并添加"确定"和"取消"按钮
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    // 连接按钮的信号到 ifAccepted 槽函数
    connect(buttonBox, &QDialogButtonBox::accepted, this, &Deltask::ifAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);

    // 设置对话框标题和大小
    setWindowTitle(tr("Time Management Program"));
    setFixedSize(300, 200);

    setLayout(layout);
}

// 槽函数，处理用户点击"确定"后的逻辑
void Deltask::ifAccepted() {
    QString id_q = lineEdit->text();
    bool ok;
    // 尝试将输入转换为整数
    int taskId = id_q.toInt(&ok);
    if (!ok) {
        // 如果转换失败，提示用户输入有效的任务 ID
        QMessageBox::information(this, tr("Notice"), tr("Please type a valid task ID."));
        return;
    }

    // 使用 TaskManager 的 deleteTask 方法尝试删除任务
    if (taskManager.deleteTask(taskId)) {
        QMessageBox::information(this, tr("Notice"), tr("Task deleted successfully!"));
        emit taskDeleted(); // 发出任务删除成功的信号
    } else {
        QMessageBox::information(this, tr("Notice"), tr("Failed to delete task."));
    }
    close(); // 关闭对话框
}
```

#### 类功能

`Deltask` 类的主要功能是提供一个简单的界面，允许用户通过输入任务 ID 来删除任务。用户可以点击"确定"来提交删除请求，如果任务删除成功，则会发出 `taskDeleted` 信号并关闭对话框；如果删除失败，则会显示相应的提示信息。该类通过 `TaskManager` 的引用访问任务管理功能，确保了与任务管理逻辑的解耦和重用。

## 接口设计

### 命令行接口

- `myschedule [command]`: 程序入口命令
  - `run`: 启动交互模式
  - `help`: 显示帮助信息

### 交互模式接口

- `addtask [name] [start time] [reminder time] [priority] [category]`: 添加任务
- `deltask [task id]`: 删除任务
- `showtasks [date]`: 显示任务
- `quit`: 退出交互模式

## 流程图
### Linux系统下的流程图
```mermaid
    graph LR

    A[用户] --> |help| C["打印帮助信息"]

    A --> |run| D["获取用户名和密码"]
    D --> E["检查用户名"];
    E --> |用户存在| F["验证登录信息"];
    F --> |成功登录| G["用户文件"];
    G --> H["加载用户任务"];
    H --> U["打印交互帮助"];
    H --> J["启动提醒线程"];
    J --> K["启动输入线程"];

    E --> |用户不存在| Q["询问注册"];
    Q --> |注册| R["注册用户"];
    R --> G;

    Q --> |不注册| T["退出程序"];

    J --> U;

    U --> V["尝试添加任务"];
    V --> W["解析参数"];
    W --> X["创建任务"];
    X --> Y["加锁"];
    Y --> Z["添加任务"];
    Z --> U

    U --> AE["尝试删除任务"];
    AE --> AF["加锁"];
    AF --> AG["删除任务"];
    AG --> U;

    U --> AI["显示任务"];
    AI --> U;

    U --> AM["退出任务"];

    H --> AP["等待提醒线程"];
    AP --> Z;

    K --> AN["等待输入线程"];
    AN --> U;

```
### Windows系统下的流程图
```mermaid
    graph LR
    BA[登录页面]-->|用户输入用户名、密码，点击Log in按键|A
    A[读取用户名、密码] --> |用户存在，密码错误| C["弹窗提示：密码错误"]
    C --> CE[退出程序]
    A --> |用户存在，密码正确| F["验证登录信息"];
    F --> |成功登录| G[加载用户日程信息];
    G --> U["新窗口：用户主界面"];
    U --> J["启动提醒线程"];
    J --> |任务到期|JE[弹窗：任务到期]

    A --> |用户不存在| Q["新窗口：是否注册"];
    Q --> |注册| R["注册用户"];
    R --> |注册成功并登录|G;

    Q --> |不注册| T["退出程序"];


    U --> |点击Add task按键| W["新窗口：Add task"];
    W --> |输入正确并点击OK按键|X["创建任务"];
    X --> |创建成功|XE[弹窗：创建成功];
    W --> |点击Cancel| WE[关闭 add task 窗口];
    XE --> WE;

    U --> |点击delete task按键|AE["新窗口: delete task"];
    AE --> |输入想要删除的任务ID并点击OK|AR[删除任务]
    AR -->|删除成功|AS[弹窗：删除成功]
    AS -->AC;
    AE --> |点击Cancel按键|AC[关闭delete task窗口]
```
## 关键技术问题说明

1. **多线程处理**:
    - 使用`pthread`库进行多线程处理，一个线程用于处理用户输入，另一个线程用于任务提醒。
    - 线程间通过`pthread_mutex_t`进行互斥锁同步，确保线程安全。

2. **时间解析与格式化**:
    - 使用`std::istringstream`和`std::get_time`进行日期时间字符串解析。
    - 使用`std::tm`结构和`mktime`函数进行时间转换与比较。

3. **任务管理**:
    - 任务添加时进行时间冲突检查，确保不会添加相同开始时间的任务。
    - 任务删除使用`std::remove_if`和`std::vector::erase`进行删除操作。
    - 任务显示时按开始时间排序，使用`std::sort`和自定义比较函数。

4. **文件操作**:
    - 任务保存和加载使用`std::ofstream`和`std::ifstream`进行文件操作。
    - 用户信息和任务数据分别存储在`data`目录下的对应文件中。

5. **错误处理**:
    - 各类操作均有详细的错误处理和提示信息，确保用户友好性。

通过上述设计，任务管理系统能够有效管理用户的任务，提供多线程提醒功能，并通过简洁的命令行接口与用户交互。
