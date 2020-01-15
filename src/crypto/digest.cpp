#include "digest.hpp"

#include <iostream>

// sha
#include <cryptopp/sha.h>

using namespace crypto;

const auto
Digest::hash(const std::string& msg) const
{
    CryptoPP::SHA512 hash;

    std::array<CryptoPP::byte, size()> digest;
    CryptoPP::StringSource(
        msg,
        true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::ArraySink(digest.data(), size()), false, size()));

    std::string hexDigest;
    CryptoPP::ArraySource(
        digest.data(),
        size(),
        true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexDigest), true));

    return hexDigest;
}

const auto
Digest::hash(const std::vector<std::byte>& bytes) const
{
    CryptoPP::SHA512 hash;

    std::array<CryptoPP::byte, size()> digest;
    CryptoPP::ArraySource(
        reinterpret_cast<const CryptoPP::byte*>(bytes.data()),
        bytes.size(),
        true,
        new CryptoPP::HashFilter(
            hash, new CryptoPP::ArraySink(digest.data(), size()), false, size()));

    std::string hexDigest;
    CryptoPP::ArraySource(
        digest.data(),
        size(),
        true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexDigest), true));

    return hexDigest;
}

const Digest
Digest::concat(const Digest& rhs) const
{
    const auto& lhs = *this;

    std::vector<std::byte> bytes;
    bytes.insert(bytes.end(), lhs.begin(), lhs.end());
    bytes.insert(bytes.end(), rhs.begin(), rhs.end());

    return Digest(bytes);
}

Digest::Digest(const std::string& msg, const bool isDigest) : ByteSet(isDigest ? msg : hash(msg))
{
}

Digest::Digest(const std::vector<std::byte>& bytes) : ByteSet(hash(bytes))
{
}
