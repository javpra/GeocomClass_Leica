cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(leica_geocom)

# Compiler specific options
 if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -pthread -Wl,--no-as-needed)
 endif()


find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

find_package(Qt5Network REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
add_definitions(${Qt5Network_DEFINITIONS})

find_package(catkin)

if(${catkin_FOUND})
	catkin_package( )
endif()



file(GLOB_RECURSE GEOCOM_LEICA "src/*.cpp" "src/*.h" "src/*.inl")

# Add Library
add_library(leica_geocom STATIC ${GEOCOM_LEICA})

target_link_libraries(leica_geocom LINK_PUBLIC ${Qt5Widgets_LIBRARIES})
target_link_libraries(leica_geocom LINK_PUBLIC ${Qt5Network_LIBRARIES})
target_include_directories(leica_geocom PUBLIC ${PROJECT_SOURCE_DIR}/src)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating API documentation with Doxygen" VERBATIM )
endif(DOXYGEN_FOUND)
