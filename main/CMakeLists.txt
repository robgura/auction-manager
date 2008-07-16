project (AuctionManager)

find_package(Qt4)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ../view/include )

add_executable(AuctionManager main.cpp)
target_link_libraries(AuctionManager view sqlite ${QT_LIBRARIES})
