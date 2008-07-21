project (AuctionManager)

find_package(Qt4)

include(${QT_USE_FILE})

file(GLOB resources *.qrc)
qt4_add_resources(resource_srcs ${resources})

add_executable(AuctionManager main.cpp ${resource_srcs})

target_link_libraries(AuctionManager view sqlite ${QT_LIBRARIES})

install(TARGETS AuctionManager DESTINATION bin)
