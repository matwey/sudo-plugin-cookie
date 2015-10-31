cmake_minimum_required (VERSION 2.8)
project(sudo-plugin-cookie C)

include(GNUInstallDirs)

file(GLOB_RECURSE SOURCES src/*.c)
add_library(cookie SHARED ${SOURCES})
set_target_properties(cookie PROPERTIES PREFIX "")
install(TARGETS cookie LIBRARY DESTINATION "${CMAKE_INSTALL_LIBEXECDIR}/sudo")
