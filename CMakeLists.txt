project (AuctionManager)

# Setup Qt4
SET( QT_USE_QTXML TRUE )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

# define common libraries
SET( commonlibs ${QT_LIBRARIES} )

# Platform Specific Rules
IF( WIN32 ) # win32
    SET( BUILD_SHARED_LIBS Off)
ELSE( WIN32 ) # not win32
    SET( BUILD_SHARED_LIBS On)
ENDIF( WIN32 )

#INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/smartmoc.cmake )
#INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/util.cmake )
#INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/depends.cmake )

# Set fileoutput paths
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
#LINK_DIRECTORIES( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )

add_subdirectory( view )
add_subdirectory(main)
