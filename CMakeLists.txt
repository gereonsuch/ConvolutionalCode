cmake_minimum_required(VERSION 2.8)

project(conv_code_cpp)

add_library(convolutionalencoder convolutionalencoder.h convolutionalencoder.cpp)
add_library(bitmanipulation bitmanipulation.h bitmanipulation.cpp)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(convolutionalencoder bitmanipulation)
target_link_libraries(${PROJECT_NAME} convolutionalencoder bitmanipulation)
