set(source_files
        agent.cpp
        )

add_library(agent-lib ${source_files})
target_link_libraries(agent-lib common collectors crypto pthread jsoncpp)

add_executable(rippledagent main.cpp)
target_link_libraries(rippledagent agent-lib)
