cmake_minimum_required(VERSION 3.25)
project(nlbrctl)

add_executable(analog nlbrctl.cpp)

file(GLOB ${PROJECT_NAME}_SOURCES "src/*.cpp")
file(GLOB ${PROJECT_NAME}_HEADERS "include/*/*.hpp")

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_link_libraries(analog PRIVATE ${PROJECT_NAME})
