cmake_minimum_required(VERSION 3.10)
project(my_app)

set(CMAKE_CXX_STANDARD 17)

# 让编译器知道头文件在哪
include_directories(include)

# 添加源文件
file(GLOB SRC_FILES "src/*.cpp")
set(SOURCES main.cpp ${SRC_FILES})

# 生成可执行文件
add_executable(my_app ${SOURCES})
