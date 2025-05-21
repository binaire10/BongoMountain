#include "AsymmetricSecureStream.hpp"

crypto::AsymmetricSecureStream::AsymmetricSecureStream(std::streambuf *s, EVP_PKEY *key, std::ios::openmode mode)
    : stream{ s }
    , ctx{ EVP_PKEY_CTX_new(key, nullptr) }
    , offset{}
{
    const auto capacity = EVP_PKEY_get_size(key);
    ibuffer.resize(capacity);
    obuffer.resize(capacity);
    if(mode & std::ios::out)
    {
        if(auto error = EVP_PKEY_encrypt_init(ctx.get()); error <= 0)
        {
            throw std::runtime_error("int encrypt");
            /*ERROR*/
        }
        setp(ibuffer.data(), ibuffer.data() + ibuffer.size());
    }
    else if(mode & std::ios::in)
    {
        if(auto error = EVP_PKEY_decrypt_init(ctx.get()); error <= 0)
        {
            throw std::runtime_error("int decrypt");
            /*ERROR*/
        }
    }

    if(auto error = EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING); error <= 0)
    {
        throw std::runtime_error("padding :/");
        /*ERROR*/
    }
}

int crypto::AsymmetricSecureStream::underflow()
{
    std::size_t length{};
    std::size_t rstream{};
    rstream = stream->sgetn(obuffer.data(), obuffer.size());
    while(rstream != traits_type::eof() && rstream != 0 && (length += rstream) < obuffer.size())
    {
        rstream = stream->sgetn(obuffer.data() + length, obuffer.size() - length);
    }
    std::size_t outLength = length;
    EVP_PKEY_decrypt(ctx.get(), reinterpret_cast<unsigned char *>(ibuffer.data()), &outLength,
                     reinterpret_cast<unsigned char *>(obuffer.data()), obuffer.size());
    setg(ibuffer.data(), ibuffer.data(), ibuffer.data() + outLength);

    return outLength == 0 ? traits_type::eof() : ibuffer.front();
}

int crypto::AsymmetricSecureStream::overflow(int ch)
{
    std::size_t outLength = obuffer.size();
    EVP_PKEY_encrypt(ctx.get(), reinterpret_cast<unsigned char *>(obuffer.data()), &outLength,
                     reinterpret_cast<unsigned char *>(pbase()), std::distance(pbase(), pptr()));

    setp(ibuffer.data(), ibuffer.data() + ibuffer.size());

    std::size_t length{};
    std::size_t rstream{};
    rstream = stream->sputn(obuffer.data(), obuffer.size());
    while(rstream != traits_type::eof() && (length += rstream) < obuffer.size())
    {
        rstream = stream->sputn(obuffer.data() + length, obuffer.size() - length);
    }

    if(ch != traits_type::eof())
    {
        *pptr() = traits_type::to_char_type(ch);
        pbump(1);
    }
    return ch;
}

int crypto::AsymmetricSecureStream::sync()
{
    overflow();
    return stream->pubsync();
}
