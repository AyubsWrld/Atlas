add_library(libatlpowmngr 
        ${CMAKE_CURRENT_SOURCE_DIR}/Private/PowerManager.cpp
    )

if(WIN32)
    target_sources(libatlpowmngr PRIVATE
        #${CMAKE_CURRENT_SOURCE_DIR}/Private/PowerManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Private/ipm_min_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Private/WMIHelper.cpp
)
endif()

target_include_directories(libatlpowmngr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public           
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private   
)
