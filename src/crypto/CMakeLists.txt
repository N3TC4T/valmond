set(source_files
        digest.cpp
        keys.cpp
        signature.cpp
        base64.cpp
        )

add_library(crypto ${source_files})
target_link_libraries(crypto cryptopp secp256k1)
