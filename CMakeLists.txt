cmake_minimum_required(VERSION 3.10)
project(svg.cc)

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

add_library(svgplot plot/src/svgplot.cc plot/src/plot.cc plot/src/scatter.cc plot/src/imshow.cc plot/src/axis-scale.cc plot/src/color.cc plot/src/colormap.cc)

add_executable(plot docs/svgplot/plot.cc)
target_link_libraries(plot svgplot)
add_executable(scatter docs/svgplot/scatter.cc)
target_link_libraries(scatter svgplot)
add_executable(imshow docs/svgplot/imshow.cc)
target_link_libraries(imshow svgplot)
add_executable(subplot docs/svgplot/subplot.cc)
target_link_libraries(subplot svgplot)
add_executable(colormap docs/svgplot/colormap.cc)
target_link_libraries(colormap svgplot)
add_executable(headeronly docs/svgplot/headeronly.cc)
add_executable(savefig docs/svgplot/savefig.cc)
target_link_libraries(savefig svgplot)
