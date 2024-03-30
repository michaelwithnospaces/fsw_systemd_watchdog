cmake_minimum_required(VERSION 2.8)
project(fsw)

add_executable(fsw fsw.cpp)
target_link_libraries(fsw PUBLIC systemd pthread)