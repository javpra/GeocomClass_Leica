cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(example_leica_geocom)

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

file(GLOB_RECURSE EXAMPLE_GEOCOM_LEICA "src/*.cpp" "src/*.h" "src/*.inl")

add_executable(example_leica_geocom ${EXAMPLE_GEOCOM_LEICA})

target_link_libraries(example_leica_geocom LINK_PUBLIC leica_geocom)
target_include_directories(example_leica_geocom PUBLIC ${PROJECT_SOURCE_DIR}/../leica_geocom/src)
target_link_libraries(example_leica_geocom LINK_PUBLIC ${Qt5Widgets_LIBRARIES})
target_link_libraries(example_leica_geocom LINK_PUBLIC ${Qt5Network_LIBRARIES})
