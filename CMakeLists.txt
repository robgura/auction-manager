project (AuctionManager)

# Setup Qt4
set(QT_USE_QTXML TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# define common libraries
set(commonlibs ${QT_LIBRARIES})

# Platform Specific Rules
if(WIN32) # win32
    set(BUILD_SHARED_LIBS Off)
else(WIN32) # not win32
    set(BUILD_SHARED_LIBS On)
endif(WIN32)

#include(${CMAKE_SOURCE_DIR}/cmake/smartmoc.cmake)
#include(${CMAKE_SOURCE_DIR}/cmake/util.cmake)
#include(${CMAKE_SOURCE_DIR}/cmake/depends.cmake)

# Set fileoutput paths
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

INCLUDE_DIRECTORIES(./)

set(CMAKE_CXX_FLAGS "-g -Wall")

#set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(sqlite)
add_subdirectory(view)
add_subdirectory(main)

