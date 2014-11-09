cmake_minimum_required(VERSION 2.8.4)

set(QUEENS "8" CACHE STRING "Number of queens to use.")
add_compile_options(-DQUEENS=${QUEENS})

add_executable(nqueens ${CMAKE_CURRENT_SOURCE_DIR}/nqueens.cpp)
target_link_libraries(nqueens pthread gomp ${Boost_LIBRARIES})
