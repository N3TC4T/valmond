include(ExternalProject)

ExternalProject_Add( project_cryptopp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/cryptopp
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/cryptopp
    CONFIGURE_COMMAND cmake <SOURCE_DIR> -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED=OFF -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_COMMAND make static
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS <BINARY_DIR>/libcryptopp.a
    PATCH_COMMAND cmake -E remove
            3way.cpp
            adler32.cpp
            # algebra.cpp
            # algparam.cpp
            arc4.cpp
            # asn.cpp
            authenc.cpp
            base32.cpp
            base64.cpp
            # basecode.cpp
            bench1.cpp
            bench2.cpp
            bfinit.cpp
            blake2.cpp
            blowfish.cpp
            blumshub.cpp
            camellia.cpp
            cast.cpp
            casts.cpp
            cbcmac.cpp
            ccm.cpp
            chacha.cpp
            channels.cpp
            cmac.cpp
            # cpu.cpp
            crc.cpp
            # cryptlib.cpp
            datatest.cpp
            default.cpp
            des.cpp
            dessp.cpp
            dh2.cpp
            dh.cpp
            # dll.cpp
            dlltest.cpp
            # dsa.cpp
            eax.cpp
            # ec2n.cpp
            # eccrypto.cpp
            # ecp.cpp
            elgamal.cpp
            emsa2.cpp
            # eprecomp.cpp
            esign.cpp
            files.cpp
            # filters.cpp
            # fips140.cpp
            fipsalgt.cpp
            fipstest.cpp
            gcm.cpp
            gf2_32.cpp
            gf256.cpp
            # gf2n.cpp
            # gfpcrypt.cpp
            gost.cpp
            gzip.cpp
            # hex.cpp
            # hmac.cpp
            # hrtimer.cpp
            ida.cpp
            idea.cpp
            # integer.cpp
            # iterhash.cpp
            # keccak.cpp
            luc.cpp
            mars.cpp
            marss.cpp
            md2.cpp
            md4.cpp
            md5.cpp
            # misc.cpp
            # modes.cpp
            # mqueue.cpp
            mqv.cpp
            # nbtheory.cpp
            network.cpp
            # oaep.cpp
            # osrng.cpp
            panama.cpp
            pch.cpp
            pkcspad.cpp
            poly1305.cpp
            # polynomi.cpp
            pssr.cpp
            # pubkey.cpp
            # queue.cpp
            rabin.cpp
            # randpool.cpp
            rc2.cpp
            rc5.cpp
            rc6.cpp
            rdrand.cpp
            # rdtables.cpp
            regtest.cpp
            # rijndael.cpp
            ripemd.cpp
            # rng.cpp
            rsa.cpp
            rw.cpp
            safer.cpp
            salsa.cpp
            seal.cpp
            seed.cpp
            serpent.cpp
            sha3.cpp
            shacal2.cpp
            # sha.cpp
            sharkbox.cpp
            shark.cpp
            simple.cpp
            skipjack.cpp
            socketft.cpp
            sosemanuk.cpp
            square.cpp
            squaretb.cpp
            # strciphr.cpp
            tea.cpp
            test.cpp
            tftables.cpp
            tiger.cpp
            tigertab.cpp
            trdlocal.cpp
            ttmac.cpp
            twofish.cpp
            validat0.cpp
            validat1.cpp
            validat2.cpp
            validat3.cpp
            vmac.cpp
            wait.cpp
            wake.cpp
            whrlpool.cpp
            # winpipes.cpp
            xtr.cpp
            xtrcrypt.cpp
            zdeflate.cpp
            zinflate.cpp
            zlib.cpp
    )

ExternalProject_Add_Step(project_cryptopp cp_cryptopp_cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/cryptopp
    COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/src/cryptopp-cmake/CMakeLists.txt . && cp ${CMAKE_CURRENT_LIST_DIR}/src/cryptopp-cmake/cryptopp-config.cmake .
    DEPENDERS configure
    )

#ExternalProject_Add_Step(project_cryptopp rm_cryptopp_cmake
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cryptopp
#    COMMAND rm CMakeLists.txt && rm cryptopp-config.cmake && rm -rf cryptopp-cmake
#    DEPENDEES install
#    )

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if(LIB64)
 set(LIBSUFFIX 64)
else()
 set(LIBSUFFIX "")
endif()

ExternalProject_Get_Property(project_cryptopp INSTALL_DIR)
add_library(cryptopp STATIC IMPORTED)
add_dependencies(cryptopp project_cryptopp)
set(CRYPTOPP_LIBRARY ${INSTALL_DIR}/lib${LIBSUFFIX}/${CMAKE_STATIC_LIBRARY_PREFIX}cryptopp${CMAKE_STATIC_LIBRARY_SUFFIX})
MESSAGE(${CRYPTOPP_LIBRARY})
set(CRYPTOPP_INCLUDE_DIR ${INSTALL_DIR}/include)

file(MAKE_DIRECTORY ${INSTALL_DIR}/include)
set_target_properties(cryptopp PROPERTIES
    IMPORTED_LOCATION ${CRYPTOPP_LIBRARY}
    "INTERFACE_INCLUDE_DIRECTORIES" ${CRYPTOPP_INCLUDE_DIR}
    )

unset(INSTALL_DIR)


