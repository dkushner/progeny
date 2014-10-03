cmake_minimum_required(VERSION 2.8.4)
set(PROJECT_NAME_STR progeny)
project(${PROJECT_NAME_STR} C CXX)

# General
#################################################
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-std=c++11 -fopenmp)
endif()

if(MSVC)
  set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()

add_definitions(${MSVC_COMPILER_DEFS})

# Set project include root to the src directory.
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/src)
set(TOOLS_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/tools)

# Google Test
#################################################
add_subdirectory(${TOOLS_PROJECTS_DIR}/gtest)

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

if(NOT WIN32)
  target_link_libraries(${PROJECT_TEST_NAME}
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    pthread
    gomp
  )
else()
  target_link_libraries(${PROJECT_TEST_NAME}
    debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
  )
  target_link_libraries(${PROJECT_TEST_NAME}
    debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
  )
endif()

add_test(test_evaluators ${PROJECT_TEST_NAME})
add_test(test_generators ${PROJECT_TEST_NAME})
add_test(test_mutators ${PROJECT_TEST_NAME})
add_test(test_pipeline ${PROJECT_TEST_NAME})

