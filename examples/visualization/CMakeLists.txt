cmake_minimum_required(VERSION 2.8.11)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#include_directories(${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
#add_definitions(${Qt5Widgets_DEFINITIONS})

QT5_WRAP_UI(UIS_HDRS mainwindow.ui)

add_executable(visualization main.cpp mainwindow.cpp ${UIS_HDRS})
qt5_use_modules(visualization Widgets Core)

target_link_libraries(visualization pthread gomp ${Boost_LIBRARIES})

