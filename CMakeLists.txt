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

# Set fileoutput paths
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/.)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# CPack

#INCLUDE(InstallRequiredSystemLibraries)

#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fantasy Football Auction Manager")
SET(CPACK_PACKAGE_VENDOR "JRG")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")


INCLUDE(CPack)

INCLUDE_DIRECTORIES(./)

set(CMAKE_CXX_FLAGS "-g -Wall")

#set(CMAKE_VERBOSE_MAKEFILE true)
#set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(sqlite)
add_subdirectory(view)
add_subdirectory(main)
