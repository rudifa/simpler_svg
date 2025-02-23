cmake_minimum_required(VERSION 3.15.0)
project(simpler_svg)

set(CMAKE_CXX_STANDARD 20)

add_executable(simpler_svg main.cpp)

# Specify the installation directory
install(TARGETS simpler_svg DESTINATION bin)
