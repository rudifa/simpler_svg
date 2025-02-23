cmake_minimum_required(VERSION 3.15.0)
project(simpler_svg)

set(CMAKE_CXX_STANDARD 20)

add_executable(simpler_svg main.cpp)

# Specify the installation directory
install(TARGETS simpler_svg DESTINATION bin)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the test executable
add_executable(
    simpler_svg_test
    ../tests/simpler_svg_test.cpp
)

# Link the test executable with Google Test and your project's source
target_link_libraries(
    simpler_svg_test
    gtest_main
)

# Add the test to CTest
include(GoogleTest)
gtest_discover_tests(simpler_svg_test)
