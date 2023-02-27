cmake_minimum_required(VERSION 3.12)
project(chapter-02-hellooop)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

find_package(librouge)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC librouge)
