project(svg.cc)
cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(Paths)
include(External)
include(Compiler)


###########################################################################################
# TARGETS
###########################################################################################
add_executable(circle examples/circle/main.cc)
add_executable(status examples/status/main.cc)
add_executable(represent examples/represent/main.cc)
