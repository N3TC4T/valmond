set(source_files
        agent.cpp
        )

add_library(valmond-lib ${source_files})
target_link_libraries(valmond-lib common collectors crypto pthread jsoncpp)

add_executable(valmond main.cpp)
target_link_libraries(valmond valmond-lib)
