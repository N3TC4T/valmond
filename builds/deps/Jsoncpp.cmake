include(ExternalProject)

ExternalProject_Add(project_jsoncpp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/jsoncpp
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/jsoncpp
    CMAKE_COMMAND ${JSONCPP_CMAKE_COMMAND}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               # Build static lib but suitable to be included in a shared lib.
               -DCMAKE_POSITION_INDEPENDENT_CODE=On
               -DJSONCPP_WITH_TESTS=Off
               -DJSONCPP_WITH_PKGCONFIG_SUPPORT=Off
               -DCMAKE_CXX_FLAGS=${JSONCCP_EXTRA_FLAGS}
    # Overwrite build and install commands to force Release build on MSVC.
    BUILD_COMMAND cmake --build <BINARY_DIR> --config Release
    INSTALL_COMMAND cmake --build <BINARY_DIR> --config Release --target install
)

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

if(LIB64)
 set(LIBSUFFIX 64)
else()
 set(LIBSUFFIX "")
endif()

# Create jsoncpp imported library
ExternalProject_Get_Property(project_jsoncpp INSTALL_DIR)
add_library(jsoncpp STATIC IMPORTED)
add_dependencies(jsoncpp project_jsoncpp)
set(JSONCPP_LIBRARY ${INSTALL_DIR}/lib${LIBSUFFIX}/${CMAKE_STATIC_LIBRARY_PREFIX}jsoncpp${CMAKE_STATIC_LIBRARY_SUFFIX})
set(JSONCPP_INCLUDE_DIR ${INSTALL_DIR}/include)

file(MAKE_DIRECTORY ${JSONCPP_INCLUDE_DIR})
set_target_properties(jsoncpp PROPERTIES
    "IMPORTED_LOCATION" ${JSONCPP_LIBRARY}
    "INTERFACE_INCLUDE_DIRECTORIES" ${JSONCPP_INCLUDE_DIR}
)
include_directories(${JSONCPP_INCLUDE_DIR})
unset(INSTALL_DIR)
