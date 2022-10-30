cmake_minimum_required(VERSION 3.15)

project(
    mocr
    LANGUAGES C CXX
    VERSION 1.0
    DESCRIPTION "C/C++ bindings for mangaocr"
)

# Options
option(EXACT_PYTHON_VERSION OFF)
option(BUILD_TESTING OFF)

# CMake Configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Compiler Options
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Dependencies
if (EXACT_PYTHON_VERSION)
    find_package(Python ${EXACT_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Development)
else()
    find_package(Python REQUIRED COMPONENTS Development)
endif()

# C Targets
set(LIBRARY_NAME_C ${PROJECT_NAME})
set(
    HDR_FILES_C
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mocr.h"
)
set(
    SRC_FILES_C
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mocr.c"
)
set(
    LIBS_C
    Python::Python
)
add_library(${LIBRARY_NAME_C} SHARED ${SRC_FILES_C})
add_library("${LIBRARY_NAME_C}_static" STATIC ${SRC_FILES_C})
set_target_properties(
    ${LIBRARY_NAME_C} PROPERTIES
    PUBLIC_HEADER ${HDR_FILES_C}
    OUTPUT_NAME ${LIBRARY_NAME_C}
    LINKER_LANGUAGE C
)
set_target_properties(
    "${LIBRARY_NAME_C}_static" PROPERTIES
    PUBLIC_HEADER ${HDR_FILES_C}
    OUTPUT_NAME ${LIBRARY_NAME_C}
    LINKER_LANGUAGE C
)
target_link_libraries(${LIBRARY_NAME_C} ${LIBS_C})
target_link_libraries("${LIBRARY_NAME_C}_static" ${LIBS_C})
install(
    TARGETS ${LIBRARY_NAME_C}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# C++ Targets
set(LIBRARY_NAME_CXX "${PROJECT_NAME}++")
set(
    HDR_FILES_CXX
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mocr++.h"
)
set(
    SRC_FILES_CXX
    "${CMAKE_CURRENT_SOURCE_DIR}/src/mocr++.cpp"
)
set(
    LIBS_CXX
    ${LIBRARY_NAME_C}
)
add_library("${LIBRARY_NAME_CXX}" SHARED ${SRC_FILES_CXX})
add_library("${LIBRARY_NAME_CXX}_static" STATIC ${SRC_FILES_CXX})
set_target_properties(
    ${LIBRARY_NAME_CXX} PROPERTIES
    PUBLIC_HEADER ${HDR_FILES_CXX}
    OUTPUT_NAME ${LIBRARY_NAME_CXX}
    LINKER_LANGUAGE CXX
)
set_target_properties(
    "${LIBRARY_NAME_CXX}_static" PROPERTIES
    PUBLIC_HEADER ${HDR_FILES_CXX}
    OUTPUT_NAME ${LIBRARY_NAME_CXX}
    LINKER_LANGUAGE CXX
)
target_link_libraries(${LIBRARY_NAME_CXX} ${LIBS_CXX})
target_link_libraries("${LIBRARY_NAME_CXX}_static" ${LIBS_CXX})
install(
    TARGETS ${LIBRARY_NAME_CXX}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Testing
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()