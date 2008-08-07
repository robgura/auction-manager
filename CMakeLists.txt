project (AuctionManager)

cmake_minimum_required(VERSION 2.6)

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

INCLUDE(InstallRequiredSystemLibraries)

#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fantasy Football Auction Manager")
SET(CPACK_PACKAGE_VENDOR "JRG")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_GENERATOR NSIS)
SET(CPACK_PACKAGE_EXECUTABLES "AuctionManager" "Auction Manager")

# unix only SET(CPACK_STRIP_FILES "bin/AuctionManager")
# unix only SET(CPACK_SOURCE_STRIP_FILES "")

INCLUDE(CPack)

INCLUDE_DIRECTORIES(./)

set(CMAKE_CXX_FLAGS "-g -Wall")

#set(CMAKE_VERBOSE_MAKEFILE true)
#set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(sqlite)
add_subdirectory(view)
add_subdirectory(main)

install(FILES "C:/Qt/4.3.5/bin/QtGui4.dll" DESTINATION bin)
install(FILES "C:/Qt/4.3.5/bin/QtCore4.dll" DESTINATION bin)
install(FILES "C:/Qt/4.3.5/bin/mingwm10.dll" DESTINATION bin)

