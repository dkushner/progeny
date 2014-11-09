cmake_minimum_required(VERSION 2.8.11)

find_package(Qt4 REQUIRED)
find_package(VTK REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(visualization_SOURCES main.cpp vtk_display.cpp)

include(${VTK_USE_FILE})

add_executable(visualization ${visualization_SOURCES} )

target_link_libraries(visualization pthread gomp ${Boost_LIBRARIES} ${VTK_LIBRARIES})

