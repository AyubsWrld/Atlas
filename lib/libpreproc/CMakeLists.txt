add_library(libpreproc) # Creates our libpreproc target.

target_sources(libpreproc 

PRIVATE
    pcm.cpp

PUBLIC
    FILE_SET HEADERS
    FILES
        pcm.hpp
)

target_link_libraries(libpreproc 
PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/libavutil.so
    ${CMAKE_SOURCE_DIR}/lib/libavformat.so
    ${CMAKE_SOURCE_DIR}/lib/libavcodec.so
)

target_include_directories(
libpreproc 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/spdlog/
)

set(libpreproc CMAKE_CXX_STANDARD 23) 

message("Libpreproc added")
