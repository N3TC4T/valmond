include_directories(${valmond_agent_SOURCE_DIR}/src)
link_directories(${valmond_agen_SOURCE_DIR}/src)

add_subdirectory(crypto)
add_subdirectory(common)
add_subdirectory(collectors)
add_subdirectory(checks)
add_subdirectory(valmond)
