cmake_minimum_required(VERSION 4.2.1)
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

if (WIN32)
    message(STATUS "Cross Compiling for Windows")
    
    # Add Z3 include directory
    target_include_directories(Atlas
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tools/Z3/mingw64/include
    )
    
    # Link against your local Z3 DLL import library (NOT the system one)
    target_link_libraries(Atlas
        PRIVATE
            ${CMAKE_SOURCE_DIR}/Tools/Z3/mingw64/lib/libz3.dll.a
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
    
    add_custom_command(TARGET Atlas POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Tools/Z3/mingw64/bin/libz3.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/bin/libstdc++-6.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/bin/libgomp-1.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/bin/libgmp-10.dll"
            "$<TARGET_FILE_DIR:Atlas>"
        COMMENT "Copying runtime DLLs to output directory"
    )
endif()
