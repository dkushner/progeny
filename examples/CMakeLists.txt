cmake_minimum_required(VERSION 2.8.4)

# Iterate through each example directory and have them build their binaries.
macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

subdirlist(SUBDIRS ${PROJECT_SOURCE_DIR}/examples)
foreach(subdir ${SUBDIRS})
  add_subdirectory(${subdir})
endforeach()
