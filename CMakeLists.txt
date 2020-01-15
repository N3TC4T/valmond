cmake_minimum_required(VERSION 3.10)
project(agent_cpp VERSION "0.1.2")


configure_file (
    "${PROJECT_SOURCE_DIR}/src/agentd/version.h.in"
    "${PROJECT_BINARY_DIR}/src/agentd/version.hpp"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -fpermissive")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


include_directories("${PROJECT_BINARY_DIR}/src/agentd/")
add_subdirectory(src)
