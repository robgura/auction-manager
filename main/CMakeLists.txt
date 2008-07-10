project (AuctionManager)

find_package(Qt4)

INCLUDE(${QT_USE_FILE})

add_executable(AuctionManager main.cpp)
target_link_libraries(AuctionManager ${QT_LIBRARIES})
