cmake_minimum_required(VERSION 2.8.4)
project(wordguess)

set(CURSES_USE_NCURSES true)
find_package(Curses)

file(GLOB wordguess_SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(wordguess ${wordguess_SRC_FILES})

add_compile_options(-std=c++11 -fopenmp)
include_directories(${CURSES_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/**/*.h)
target_link_libraries(wordguess pthread gomp)


