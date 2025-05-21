#ifndef SYMMETRICSECURESTREAM_HPP
#define SYMMETRICSECURESTREAM_HPP

#include <streambuf>
#include <ios>
#include <vector>
#include <span>

#include "Crypto.hpp"

namespace crypto
{
    class BM_CORE_DCL SymmetricSecureStream final : public std::streambuf
    {
    public:
        SymmetricSecureStream(std::streambuf                *s,
                              const EVP_CIPHER              *algo,
                              std::size_t                    bufferSize,
                              const std::span<std::uint8_t> &key,
                              const std::span<std::uint8_t> &iv,
                              std::ios::openmode             mode);

        SymmetricSecureStream(crypto::EVP_CIPHER_CTX_ptr   &&ctx_,
                              std::streambuf                *s,
                              const EVP_CIPHER              *algo,
                              std::size_t                    bufferSize,
                              const std::span<std::uint8_t> &key,
                              const std::span<std::uint8_t> &iv,
                              std::ios::openmode             mode);

        ~SymmetricSecureStream() override;

        int_type underflow() override;

        int_type overflow(int_type ch = traits_type::eof()) override;


        int sync() override;

    private:
        bool                   isEncode;
        std::streambuf        *stream;
        EVP_CIPHER_CTX_ptr     ctx;
        std::vector<char_type> ibuffer;
        std::vector<char_type> obuffer;
    };
}// namespace crypto

#endif// SYMMETRICSECURESTREAM_HPP
