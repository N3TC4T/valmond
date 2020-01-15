set(source_files
        process.cpp
        validation.cpp
        )

add_library(checks ${source_files})
target_link_libraries(checks)
