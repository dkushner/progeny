# Michael Aaron Safyan (michaelsafyan@gmail.com). Copyright (C) 2009. Simplified BSD License.

#
# This CMake package creates a Doxygen documentation target.
#
find_package(Doxygen)
IF (DOXYGEN_FOUND)
    IF (EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
        add_custom_target(
            doxygen
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ../
            COMMENT "Generating doxygen project documentation." VERBATIM
        )
        add_custom_target(
            documentation
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ../
            COMMENT "Generating doxygen project documentation." VERBATIM
        )
        add_custom_target(
            docs
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ../
            COMMENT "Generating doxygen project documentation." VERBATIM
        )
    ELSE (EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
        add_custom_target(doxygen COMMENT "Doxyfile not found. Please generate a doxygen configuration file to use this target." VERBATIM)
        add_custom_target(documentation COMMENT "Doxyfile not found. Please generate a doxygen configuration file to use this target." VERBATIM)
        add_custom_target(docs COMMENT "Doxyfile not found. Please generate a doxygen configuration file to use this target." VERBATIM)
    ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
ELSE (DOXYGEN_FOUND)
    add_custom_target(doxygen COMMENT "Doxygen not found. Please install doxygen to use this target." VERBATIM)
    add_custom_target(documentation COMMENT "Doxygen not found. Please install doxygen to use this target." VERBATIM)
    add_custom_target(docs COMMENT "Doxygen not found. Please install doxygen to use this target." VERBATIM)
ENDIF (DOXYGEN_FOUND)
