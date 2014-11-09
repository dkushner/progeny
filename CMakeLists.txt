cmake_minimum_required(VERSION 2.8.4)
set(PROJECT_NAME_STR progeny)
project(${PROJECT_NAME_STR} C CXX)

# General
#################################################
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-std=c++11 -fopenmp -g -Wall)
endif()

if(MSVC)
  set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()

# Set project include root to the src directory.
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/src)
set(TOOLS_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/tools) 

# Load Boost libs.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.54.0 
  COMPONENTS 
    mpi
    thread
    system
    program_options
    signals
  REQUIRED
)

# Google Test
#################################################
add_subdirectory(${TOOLS_PROJECTS_DIR}/gtest)

# Test Build
#################################################
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES} ${Boost_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

if(NOT WIN32)
  target_link_libraries(${PROJECT_TEST_NAME}
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    pthread
    gomp
    ${Boost_LIBRARIES}
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


# 
# Examples
################################################
add_subdirectory(examples)

