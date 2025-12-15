cmake_minimum_required(VERSION 3.16)

project(filemanager_tests LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Test)

enable_testing()

add_executable(${PROJECT_NAME}
    tests_src/main.cpp
    tests_src/test.cpp
    tests_src/test.h
    ../src/filemanager.cpp
    ../src/mainwindow.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Test)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

