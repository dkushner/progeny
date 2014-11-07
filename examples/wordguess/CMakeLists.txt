cmake_minimum_required(VERSION 2.8.4)

add_executable(wordguess ${CMAKE_CURRENT_SOURCE_DIR}/wordguess.cpp)
target_link_libraries(wordguess pthread gomp ${Boost_LIBRARIES})

