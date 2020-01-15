#include "keys.hpp"

// System includes
#include <iomanip>
#include <sstream>

using namespace crypto;

SecretKey::SecretKey(const std::string& str) : ByteSet(str)
{
}

const auto
PublicKey::getContextKeys()
{
    static auto context = std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)>(
        secp256k1_context_create(SECP256K1_CONTEXT_SIGN), &secp256k1_context_destroy);
    return context.get();
}

const auto
PublicKey::deriveFromSecretKey(const SecretKey& secretKey)
{
    const auto context = getContextKeys();

    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(
            context, &pubkey, reinterpret_cast<const unsigned char*>(secretKey.raw())))
        throw std::runtime_error("Invalid secret key");

    std::array<std::byte, 65u> pubkey_serialized;
    auto outputlen = pubkey_serialized.size();
    secp256k1_ec_pubkey_serialize(
        context,
        reinterpret_cast<unsigned char*>(pubkey_serialized.data()),
        &outputlen,
        &pubkey,
        SECP256K1_EC_UNCOMPRESSED);

    std::stringstream buf_pubkey;
    buf_pubkey << std::hex << std::uppercase << std::setfill('0');
    auto it = pubkey_serialized.begin();
    for (++it; it != pubkey_serialized.end(); ++it)
        buf_pubkey << std::setw(2) << static_cast<int>(*it);

    return PublicKey(buf_pubkey.str());
}

PublicKey::PublicKey(const std::string& str) : ByteSet(str)
{
}

PublicKey::PublicKey(const SecretKey& secretKey) : ByteSet(deriveFromSecretKey(secretKey))
{
}
