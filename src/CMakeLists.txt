# 添加静态库
add_library(Mylibrary STATIC
    UserManager.cpp
    Task.cpp
    TaskManager.cpp
)

# 添加可执行文件
add_executable(MyExecutable main.cpp)

# 将 MyExecutable 可执行文件链接到 Mylibrary 静态库
target_link_libraries(MyExecutable PRIVATE Mylibrary)

# 安装目标
install(TARGETS MyExecutable Mylibrary
    RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
