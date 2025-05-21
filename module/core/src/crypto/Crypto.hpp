#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <openssl/pkcs12.h>

#include <memory>
#include "../core/config.hpp"
#include "../core/resource.hpp"

namespace crypto
{
    using PKCS12_ptr         = std::unique_ptr<PKCS12, core::labda_deleter<PKCS12_free>>;
    using EVP_PKEY_ptr       = std::unique_ptr<EVP_PKEY, core::labda_deleter<EVP_PKEY_free>>;
    using EVP_CIPHER_CTX_ptr = std::unique_ptr<EVP_CIPHER_CTX, core::labda_deleter<EVP_CIPHER_CTX_free>>;
    using EVP_MD_CTX_ptr     = std::unique_ptr<EVP_MD_CTX, core::labda_deleter<EVP_MD_CTX_free>>;
    using EVP_PKEY_CTX_ptr   = std::unique_ptr<EVP_PKEY_CTX, core::labda_deleter<EVP_PKEY_CTX_free>>;

    BM_CORE_DCL PKCS12_ptr   loadPSCK12(std::string_view filename);
    BM_CORE_DCL EVP_PKEY_ptr getPrivateKey(const PKCS12_ptr &p12, std::string_view password);
}// namespace crypto

#endif// CRYPTO_HPP
