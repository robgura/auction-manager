#SLICKER_DEPENDS_ON(linkable util audio eval model eventfilters osabstraction graphics framework)

set(LIB_NAME view)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

file(GLOB headers *.h)
file(GLOB srcs src/*.cpp src/*.c)
file(GLOB uis ui/*.ui)

#qt_wrap_cpp(${LIB_NAME} moc_files ${headers})
#qt_wrap_ui(${LIB_NAME} ui_headers ui_srcs ${uis})

qt4_wrap_ui(ui_headers ${uis})
qt4_wrap_cpp(moc_files ${headers})

ADD_LIBRARY(${LIB_NAME} ${ui_headers} ${srcs} ${moc_files})

install(TARGETS ${LIB_NAME} DESTINATION lib)
