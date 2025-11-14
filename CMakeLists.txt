cmake_minimum_required(VERSION 3.15...4.1.2)
project(Atlas LANGUAGES C CXX)

add_subdirectory(./lib/libpreproc)

add_executable(atlas src/main.cpp)

target_link_libraries(atlas PUBLIC libpreproc)




