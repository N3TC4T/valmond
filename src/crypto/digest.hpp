#pragma once

// Project includes
#include "byteset.hpp"

#define DEFINE_DIGEST_BYTES 32u

namespace crypto {

class Digest : public byteset<DEFINE_DIGEST_BYTES>
{
private:
    [[nodiscard]] const auto
    hash(const std::string& msg) const;

    [[nodiscard]] const auto
    hash(const std::vector<std::byte>& bytes) const;

    [[nodiscard]] const Digest
    concat(const Digest& rhs) const;

public:
    /**
     * Default: the sha512 hash of 32-byte zero.
     */
    Digest();

    explicit Digest(const std::array<std::byte, DEFINE_DIGEST_BYTES>& bytes) : ByteSet(bytes)
    {
    }

    explicit Digest(const std::string& msg, bool isDigest = false);

    explicit Digest(const std::vector<std::byte>& bytes);

    friend const auto
    operator+(const Digest& lhs, const Digest& rhs)
    {
        return lhs.concat(rhs);
    }
};

}  // namespace crypto
