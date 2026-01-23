add_library(libatlproc)

target_sources(libatlproc
    PRIVATE
        Private/_iproc_min_impl.cpp
        Private/ProcCommon.cpp
)

if(WIN32)
    target_sources(libatlproc PRIVATE
        Private/Win32Process.cpp
        Private/_proccommon_w32.cpp
    )
elseif(UNIX)
    message("libatl_w32")
    target_sources(libatlproc PRIVATE
        Private/UnixProcess.cpp
        Private/_proccommon_linux.cpp
    )
endif()

target_include_directories(libatlproc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)
