add_library(libatllangproc)

target_include_directories(libatllangproc 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_sources(
    libatllangproc
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Private/LanguageProcessing.cpp
)
