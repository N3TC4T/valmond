include(ExternalProject)

ExternalProject_Add(project_glog
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/glog
  GIT_REPOSITORY https://github.com/google/glog
  GIT_TAG v0.4.0
  CMAKE_ARGS
    -DCMAKE_CXX_FLAGS="-Wl,--eh-frame-hdr"
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

# Create glog imported library
ExternalProject_Get_Property(project_glog INSTALL_DIR)
add_library(glog STATIC IMPORTED)
add_dependencies(glog project_glog)

set(GLOG_INCLUDE_DIR ${INSTALL_DIR}/include)
set(GLOG_LIBRARY ${INSTALL_DIR}/lib/libglog.a)

file(MAKE_DIRECTORY ${GLOG_INCLUDE_DIR})
set_target_properties(glog PROPERTIES
    "IMPORTED_LOCATION" ${GLOG_LIBRARY}
    "INTERFACE_INCLUDE_DIRECTORIES" ${GLOG_INCLUDE_DIR}
)
include_directories(${GLOG_INCLUDE_DIR})
unset(INSTALL_DIR)
