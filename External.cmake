##################################################################################
# INSTALLED LIBRARIES
##################################################################################
find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIR})
	list(APPEND svg_cpp_plot_libs ${PNG_LIBRARY})
	list(APPEND svg_cpp_plot_defs "USE_PNG")
endif(PNG_FOUND)

######################################################################
# EXTERNAL LIBRARIES (GitHub etc...) added as submodules
######################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext)







