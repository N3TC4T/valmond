cmake_minimum_required(VERSION 3.10)
project(agent_cpp VERSION "0.1.2")

SET (LINUX true)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET (CMAKE_CXX_FLAGS "-std=c++11 -fopenmp -Wall -fpermissive ${CMAKE_CXX_FLAGS}")

# Version
configure_file (
    "${PROJECT_SOURCE_DIR}/src/agentd/version.h.in"
    "${PROJECT_BINARY_DIR}/src/agentd/version.hpp"
    )

# Output path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/builds/bin)

# Version
include_directories("${PROJECT_BINARY_DIR}/src/agentd/")

# deps
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/builds/deps")
include(Cryptopp)
include(Secp256k1)
include(Jsoncpp)

# main source directory
add_subdirectory(src)
