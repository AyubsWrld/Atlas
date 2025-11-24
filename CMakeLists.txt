cmake_minimum_required(VERSION 3.15...4.1.2)
project(Atlas LANGUAGES C CXX)

add_subdirectory(./lib/libpreproc)
add_subdirectory(external/ftxui)
 
add_executable(atlas src/main.cpp)

target_link_libraries(atlas
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
set(CMAKE_CXX_STANDARD 23)

target_link_libraries(atlas PUBLIC libpreproc)




