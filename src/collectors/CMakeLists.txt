set(source_files
        all.cpp
        cpu.cpp
        load.cpp
        host.cpp
        memory.cpp
        disk.cpp
        network.cpp
        validator.cpp
        )

add_library(collectors ${source_files})
target_link_libraries(collectors checks common jsoncpp blkid ext2fs)
