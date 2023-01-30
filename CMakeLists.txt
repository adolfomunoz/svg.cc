project(svg.cc)
cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(Paths)
include(External)
include(Compiler)

###########################################################################################
# TARGETS
###########################################################################################
add_executable(status main/status/main.cc)
add_executable(represent main/represent/main.cc)
add_executable(figure-inset main/figure-inset/main.cc)

add_library(svgplot plot/src/plot.cc plot/src/axis-scale.cc)
