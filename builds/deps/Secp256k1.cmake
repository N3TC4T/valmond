include(ExternalProject)

ExternalProject_Add( project_secp256k1
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/secp256k1
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/secp256k1
    CONFIGURE_COMMAND ${CMAKE_CURRENT_LIST_DIR}/src/secp256k1/configure --disable-benchmark --disable-tests --disable-exhaustive-tests --enable-module-recovery --enable-module-rangeproof --enable-module-generator --enable-experimental --with-bignum=no --with-field=32bit --with-scalar=32bit
    BUILD_COMMAND make clean && make
    INSTALL_COMMAND DESTDIR=<INSTALL_DIR> make install
    BUILD_BYPRODUCTS <BINARY_DIR>/.libs/libsecp256k1.a
    )

ExternalProject_Add_Step(project_secp256k1 autogen
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/secp256k1
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/src/secp256k1/autogen.sh
    DEPENDERS configure
    )

ExternalProject_Get_Property(project_secp256k1 INSTALL_DIR)
add_library(secp256k1 STATIC IMPORTED)
add_dependencies(secp256k1 project_secp256k1)
file(MAKE_DIRECTORY ${INSTALL_DIR}/usr/local/include)
set_target_properties(secp256k1 PROPERTIES
    IMPORTED_LOCATION "${INSTALL_DIR}/usr/local/lib/libsecp256k1.a"
    INTERFACE_INCLUDE_DIRECTORIES "${INSTALL_DIR}/usr/local/include"
    )
unset(INSTALL_DIR)
