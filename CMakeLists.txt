cmake_minimum_required(VERSION 3.15...4.1.2)
project(Atlas LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(./lib/libpreproc)
add_subdirectory(external/ftxui)
 
add_executable(atlas src/main.cpp)


target_link_libraries(atlas
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")


target_link_libraries(atlas PUBLIC libpreproc)




