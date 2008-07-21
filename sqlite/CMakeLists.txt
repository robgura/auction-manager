#SLICKER_DEPENDS_ON(linkable util audio eval model eventfilters osabstraction graphics framework)

set(LIB_NAME sqlite)

include_directories(./)

file(GLOB headers ./*.h)
file(GLOB srcs ./*.c)

ADD_LIBRARY(${LIB_NAME} ${srcs})

install(TARGETS ${LIB_NAME} DESTINATION lib)
