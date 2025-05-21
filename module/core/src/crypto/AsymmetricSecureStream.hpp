#ifndef ASYMMETRICSECURESTREAM_HPP
#define ASYMMETRICSECURESTREAM_HPP

#include <streambuf>
#include <ios>
#include <vector>

#include "Crypto.hpp"

namespace crypto
{
    class BM_CORE_DCL AsymmetricSecureStream : public std::streambuf
    {
    public:
        AsymmetricSecureStream(std::streambuf *s, EVP_PKEY *key, std::ios::openmode mode);

        int_type underflow() override;

        int_type overflow(int_type ch = traits_type::eof()) override;

        int sync() override;


    private:
        std::streambuf        *stream;
        EVP_PKEY_CTX_ptr       ctx;
        std::vector<char_type> ibuffer;
        std::vector<char_type> obuffer;
        std::size_t            offset;
    };
}// namespace crypto

#endif// ASYMMETRICSECURESTREAM_HPP
