#pragma once

// System includes
#include <iomanip>
#include <iostream>

// Library includes
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

namespace crypto {

/**
 * Like a bitset container, but for bytes. Fixed-size blob of bytes.
 */
template <size_t N>
class ByteSet
{
private:
    static constexpr auto _hexes = std::string_view("0123456789ABCDEF");

    std::array<std::byte, N> _data;

    [[nodiscard]] static const auto
    hexToBytes(const std::string& str)
    {
        std::array<std::byte, N> data;

        if (str.length() % 2u != 0u)
            throw std::invalid_argument("hex string must have an even number of hexes");
        if (str.length() / 2u != N)
            throw std::invalid_argument("attempting to convert from hex string with invalid size");

        for (const auto& c : str)
        {
            if (std::find(_hexes.begin(), _hexes.end(), c) == _hexes.end())
                throw std::invalid_argument("invalid hex char");
        }

        CryptoPP::StringSource(
            str,
            true,
            new CryptoPP::HexDecoder(
                new CryptoPP::ArraySink(reinterpret_cast<CryptoPP::byte*>(data.data()), N)));

        return data;
    }

public:
    explicit ByteSet(const std::string& str) : _data(hexToBytes(str))
    {
    }

    explicit ByteSet(const std::array<std::byte, N>& bytes) : _data(bytes)
    {
    }

    [[nodiscard]] const auto
    toHex() const
    {
        std::stringstream buf;
        buf << std::hex << std::setfill('0');
        for (const auto& c : this->_data)
            buf << std::setw(2) << static_cast<int>(c);

        return buf.str();
    }

    [[nodiscard]] static constexpr auto
    size()
    {
        return N;
    }

    [[nodiscard]] const auto
    raw() const
    {
        return this->_data.data();
    }

    [[nodiscard]] const auto
    begin() const
    {
        return this->_data.begin();
    }
    [[nodiscard]] const auto
    end() const
    {
        return this->_data.end();
    }

    friend bool
    operator==(const ByteSet<N>& lhs, const ByteSet<N>& rhs)
    {
        return std::memcmp(lhs.raw(), rhs.raw(), N) == 0;
    }

    friend bool
    operator!=(const ByteSet<N>& lhs, const ByteSet<N>& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool
    operator<(const ByteSet<N>& lhs, const ByteSet<N>& rhs)
    {
        return std::memcmp(lhs.raw(), rhs.raw(), N) < 0;
    }

    friend bool
    operator>(const ByteSet<N>& lhs, const ByteSet<N>& rhs)
    {
        return std::memcmp(lhs.raw(), rhs.raw(), N) > 0;
    }

    friend std::ostream&
    operator<<(std::ostream& os, const ByteSet<N>& o)
    {
        os << o.toHex();
        return os;
    }
};

// Make it look like bitset
template <size_t N>
using byteset = ByteSet<N>;

}  // namespace crypto
