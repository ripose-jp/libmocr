# Set the standard for tests
set(CMAKE_CXX_STANDARD 20)

# Boilerplate for importing Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/3026483ae575e2de942db5e760cf95e973308dd5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Includes
include(GoogleTest)

# Variables
set(
    TEST_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/test/lib"
)

# Tests
add_executable(
    mocr_test
    mocr_test.cpp
)
target_include_directories(mocr_test PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(
    mocr_test
    ${LIBRARY_NAME_C}
    GTest::gtest_main
)
gtest_discover_tests(
    mocr_test
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
)

add_executable(
    mocrxx_test
    mocrxx_test.cpp
)
target_include_directories(mocrxx_test PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(
    mocrxx_test
    ${LIBRARY_NAME_CXX}
    GTest::gtest_main
)
gtest_discover_tests(
    mocrxx_test
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test"
)
