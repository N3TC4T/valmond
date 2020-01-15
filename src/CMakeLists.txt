include_directories(${agent_cpp_SOURCE_DIR}/src)
link_directories(${agent_cpp_SOURCE_DIR}/src)

add_subdirectory(crypto)
add_subdirectory(common)
add_subdirectory(collectors)
add_subdirectory(checks)
add_subdirectory(agentd)
