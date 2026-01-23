cmake_minimum_required(VERSION 3.20)
project(Atlas LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Source/PowerManagement)
add_subdirectory(Source/Process)

add_executable(Atlas Source/Application/main.cpp)

target_link_libraries(Atlas
    PRIVATE
        libatlpowmngr
        libatlproc
)

message(STATUS "${CMKAE_SOURCE_DIR}")

if (WIN32)
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll.lib" ".a")
    target_link_libraries(Atlas
        PRIVATE
            kernel32
            user32
            gdi32
            winspool
            comdlg32
            advapi32
            shell32
            ole32
            oleaut32
            uuid
            odbc32
            odbccp32
            setupapi
            wbemuuid
            shlwapi
    )
    
    set(Z3_ROOT ${CMAKE_SOURCE_DIR}/Tools/Z3/mingw_x86_64-win)
    set(Z3_LIBRARY ${Z3_ROOT}/bin/libz3.dll.a)
    
    if(NOT EXISTS ${Z3_LIBRARY})
        message(FATAL_ERROR "Z3 library not found: ${Z3_LIBRARY}")
    endif()
    
    target_link_libraries(Atlas PRIVATE ${Z3_LIBRARY})
    target_include_directories(Atlas PRIVATE ${Z3_ROOT}/include)
    
    # add_custom_command(TARGET Atlas POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #         ${Z3_ROOT}/bin/libz3.dll
    #         $<TARGET_FILE_DIR:Atlas>
    # )
endif()

if (CMAKE_CROSSCOMPILING)
  if (CMAKE_SYSTEM_NAME EQUAL Windows)
      message(STATUS "Cross Compiling")
  endif()
endif()

target_compile_definitions(Atlas
    PRIVATE
        _GNU_SOURCE
        EXPERIMENTAL_KEY_INSTRUCTIONS
)
