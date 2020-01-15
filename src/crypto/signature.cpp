#include "signature.hpp"

#include <iomanip>
#include <iostream>

// secp256k1
#include <secp256k1_recovery.h>

using namespace crypto;

const auto
Signature::getContextSign()
{
    static auto context = std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)>(
        secp256k1_context_create(SECP256K1_CONTEXT_SIGN), &secp256k1_context_destroy);
    return context.get();
}

const auto
Signature::getContextVerify()
{
    static auto context = std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)>(
        secp256k1_context_create(SECP256K1_CONTEXT_VERIFY), &secp256k1_context_destroy);
    return context.get();
}

const auto
Signature::sign(const SecretKey& secretKey, const Digest& digest)
{
    const auto context = getContextSign();

    secp256k1_ecdsa_recoverable_signature sig;
    if (!secp256k1_ecdsa_sign_recoverable(
            context,
            &sig,
            reinterpret_cast<const unsigned char*>(digest.raw()),
            reinterpret_cast<const unsigned char*>(secretKey.raw()),
            nullptr,
            nullptr))
        throw std::runtime_error("Could not sign message digest: " + digest.toHex());

    std::array<std::byte, 64u> rs;
    int v = 0;
    secp256k1_ecdsa_recoverable_signature_serialize_compact(
        context, reinterpret_cast<unsigned char*>(rs.data()), &v, &sig);
    v += 27;

    std::stringstream buf;
    buf << std::hex << std::uppercase << std::setfill('0');
    for (const auto& c : rs)
        buf << std::setw(2) << static_cast<int>(c);
    buf << std::setw(2) << v;

    return buf.str();
}

Signature::Signature(const std::string& sig) : ByteSet(sig)
{
}

Signature::Signature(const SecretKey& secretKey, const std::vector<std::byte>& msg)
    : Signature(sign(secretKey, Digest(msg)))
{
}

Signature::Signature(const SecretKey& secretKey, const std::string& msg)
    : Signature(sign(secretKey, Digest(msg)))
{
}
