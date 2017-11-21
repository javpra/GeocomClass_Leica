cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(LEICA)

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


file(GLOB_RECURSE GEOCOM_LEICA "src/*.cpp" "src/*.h" "src/*.inl")

# Add Library
add_library(LeicaGeocom STATIC ${GEOCOM_LEICA})

target_link_libraries(LeicaGeocom LINK_PUBLIC ${Qt5Widgets_LIBRARIES})
target_link_libraries(LeicaGeocom LINK_PUBLIC ${Qt5Network_LIBRARIES})
target_include_directories(LeicaGeocom PUBLIC ${PROJECT_SOURCE_DIR}/src)
