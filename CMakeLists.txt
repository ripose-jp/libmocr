cmake_minimum_required(VERSION 3.15...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    mocr
    LANGUAGES C CXX
    VERSION 1.0
    DESCRIPTION "C/C++ bindings for mangaocr"
)

# Options
option(EXACT_PYTHON_VERSION "Specify the exact Python version to link to" OFF)
option(BUILD_TESTING "Build test suites" OFF)

if(MSVC)
    set(MOCR_COMPILE_FLAGS "/W4" "/WX")
else()
    set(MOCR_COMPILE_FLAGS "-Wall" "-Wextra" "-Wpedantic" "-Werror")
endif()

# Dependencies
if (EXACT_PYTHON_VERSION)
    find_package(Python "${EXACT_PYTHON_VERSION}" EXACT REQUIRED COMPONENTS Development)
else()
    find_package(Python REQUIRED COMPONENTS Development)
endif()

# C Targets
set(MOCR_LIBRARY_NAME_C ${PROJECT_NAME})
set(
    MOCR_HDR_FILES_C
    "${PROJECT_SOURCE_DIR}/src/mocr.h"
)
set(
    MOCR_SRC_FILES_C
    "${PROJECT_SOURCE_DIR}/src/mocr.c"
)
set(
    MOCR_LIBS_C
    Python::Python
)
add_library(${MOCR_LIBRARY_NAME_C} SHARED ${MOCR_SRC_FILES_C})
add_library("${MOCR_LIBRARY_NAME_C}_static" STATIC ${MOCR_SRC_FILES_C})
target_include_directories(
    "${MOCR_LIBRARY_NAME_C}" PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
target_include_directories(
    "${MOCR_LIBRARY_NAME_C}_static" PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
    ${MOCR_LIBRARY_NAME_C} PROPERTIES
    PUBLIC_HEADER ${MOCR_HDR_FILES_C}
    OUTPUT_NAME ${MOCR_LIBRARY_NAME_C}
    LINKER_LANGUAGE C
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
set_target_properties(
    "${MOCR_LIBRARY_NAME_C}_static" PROPERTIES
    PUBLIC_HEADER ${MOCR_HDR_FILES_C}
    OUTPUT_NAME ${MOCR_LIBRARY_NAME_C}
    LINKER_LANGUAGE C
)
target_compile_features(${MOCR_LIBRARY_NAME_C} PUBLIC c_std_99)
target_compile_features("${MOCR_LIBRARY_NAME_C}_static" PUBLIC c_std_99)
target_compile_options(
    ${MOCR_LIBRARY_NAME_C} PRIVATE ${MOCR_COMPILE_FLAGS}
)
target_compile_options(
    "${MOCR_LIBRARY_NAME_C}_static" PRIVATE ${MOCR_COMPILE_FLAGS}
)
target_link_libraries(${MOCR_LIBRARY_NAME_C} PUBLIC ${MOCR_LIBS_C})
target_link_libraries("${MOCR_LIBRARY_NAME_C}_static" PUBLIC ${MOCR_LIBS_C})
install(
    TARGETS ${MOCR_LIBRARY_NAME_C}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# C++ Targets
set(MOCR_LIBRARY_NAME_CXX "${PROJECT_NAME}++")
set(
    MOCR_HDR_FILES_CXX
    "${PROJECT_SOURCE_DIR}/src/mocr++.h"
)
set(
    MOCR_SRC_FILES_CXX
    "${PROJECT_SOURCE_DIR}/src/mocr++.cpp"
)
set(
    MOCR_LIBS_CXX
    ${MOCR_LIBRARY_NAME_C}
)
add_library("${MOCR_LIBRARY_NAME_CXX}" SHARED ${MOCR_SRC_FILES_CXX})
add_library("${MOCR_LIBRARY_NAME_CXX}_static" STATIC ${MOCR_SRC_FILES_CXX})
target_include_directories(
    "${MOCR_LIBRARY_NAME_CXX}" PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
target_include_directories(
    "${MOCR_LIBRARY_NAME_CXX}_static" PUBLIC "${PROJECT_SOURCE_DIR}/src"
)
set_target_properties(
    ${MOCR_LIBRARY_NAME_CXX} PROPERTIES
    PUBLIC_HEADER ${MOCR_HDR_FILES_CXX}
    OUTPUT_NAME ${MOCR_LIBRARY_NAME_CXX}
    LINKER_LANGUAGE CXX
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
set_target_properties(
    "${MOCR_LIBRARY_NAME_CXX}_static" PROPERTIES
    PUBLIC_HEADER ${MOCR_HDR_FILES_CXX}
    OUTPUT_NAME ${MOCR_LIBRARY_NAME_CXX}
    LINKER_LANGUAGE CXX
)
target_compile_features(${MOCR_LIBRARY_NAME_CXX} PUBLIC cxx_std_11)
target_compile_features("${MOCR_LIBRARY_NAME_CXX}_static" PUBLIC cxx_std_11)
target_compile_options(
    ${MOCR_LIBRARY_NAME_CXX} PRIVATE ${MOCR_COMPILE_FLAGS}
)
target_compile_options(
    "${MOCR_LIBRARY_NAME_CXX}_static" PRIVATE ${MOCR_COMPILE_FLAGS}
)
target_link_libraries(${MOCR_LIBRARY_NAME_CXX} PUBLIC ${MOCR_LIBS_CXX})
target_link_libraries("${MOCR_LIBRARY_NAME_CXX}_static" PUBLIC ${MOCR_LIBS_CXX})
install(
    TARGETS ${MOCR_LIBRARY_NAME_CXX}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Testing
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
