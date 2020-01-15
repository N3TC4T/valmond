#pragma once

// Library includes
#include <secp256k1.h>

// Project includes
#include "byteset.hpp"
#include "digest.hpp"

#define DEFINE_SECRET_KEY_BYTES 32u
#define DEFINE_PUBLIC_KEY_BYTES 64u

namespace crypto {
// TODO need to zero out secret key on destroy
class SecretKey : public byteset<DEFINE_SECRET_KEY_BYTES>
{
public:
    explicit SecretKey(const std::string& str);
};

class PublicKey : public byteset<DEFINE_PUBLIC_KEY_BYTES>
{
private:
    [[nodiscard]] static const auto
    getContextKeys();

    [[nodiscard]] static const auto
    deriveFromSecretKey(const SecretKey& secretKey);

public:
    explicit PublicKey(const std::string& str);

    explicit PublicKey(const SecretKey& secretKey);
};

}  // namespace crypto
