project (AuctionManager)

find_package(Qt4)

include(${QT_USE_FILE})

add_executable(AuctionManager main.cpp)
target_link_libraries(AuctionManager view sqlite ${QT_LIBRARIES})
