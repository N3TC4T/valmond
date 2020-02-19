set(source_files
        config.cpp
        daemon.cpp
        http.cpp
        utils.cpp
        log.hpp
        )

add_library(common ${source_files})
target_link_libraries(common crypto jsoncpp glog curl)
