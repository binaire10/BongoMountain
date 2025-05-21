#include "SymmetricSecureStream.hpp"

crypto::SymmetricSecureStream::SymmetricSecureStream(std::streambuf                *s,
                                                     const EVP_CIPHER              *algo,
                                                     std::size_t                    bufferSize,
                                                     const std::span<std::uint8_t> &key,
                                                     const std::span<std::uint8_t> &iv,
                                                     std::ios::openmode             mode)
    : SymmetricSecureStream(crypto::EVP_CIPHER_CTX_ptr{ EVP_CIPHER_CTX_new() }, s, algo, bufferSize, key, iv, mode)
{}

crypto::SymmetricSecureStream::SymmetricSecureStream(crypto::EVP_CIPHER_CTX_ptr   &&ctx_,
                                                     std::streambuf                *s,
                                                     const EVP_CIPHER              *algo,
                                                     std::size_t                    bufferSize,
                                                     const std::span<std::uint8_t> &key,
                                                     const std::span<std::uint8_t> &iv,
                                                     std::ios::openmode             mode)
    : stream{ s }
    , ctx{ std::move(ctx_) }
{
    isEncode = mode & std::ios::out;
    EVP_CIPHER_CTX_init(ctx.get());

    if(auto error = EVP_CipherInit_ex(ctx.get(), algo, nullptr, nullptr, nullptr, isEncode); error != 1)
    {
        throw std::runtime_error("int encrypt");
        /*ERROR*/
    }
    const auto ivLength    = EVP_CIPHER_CTX_get_iv_length(ctx.get());
    const auto keyLength   = EVP_CIPHER_CTX_get_key_length(ctx.get());
    const auto blockLength = EVP_CIPHER_CTX_get_block_size(ctx.get());
    OPENSSL_assert(keyLength == key.size());
    OPENSSL_assert(ivLength == iv.size());

    if(auto error = EVP_CipherInit_ex(ctx.get(), nullptr, nullptr, key.data(), iv.data(), isEncode); error != 1)
    {
        throw std::runtime_error("int encrypt");
        /*ERROR*/
    }
    ibuffer.resize(blockLength * bufferSize + (bufferSize < 1));
    obuffer.resize(blockLength * bufferSize + EVP_MAX_BLOCK_LENGTH);

    if(mode & std::ios::out)
    {
        setp(ibuffer.data(), ibuffer.data() + ibuffer.size());
    }
}

crypto::SymmetricSecureStream::~SymmetricSecureStream()
{
    if(isEncode)
    {
        overflow();
        int outLength = obuffer.size();
        EVP_CipherFinal_ex(ctx.get(), reinterpret_cast<unsigned char *>(obuffer.data()), &outLength);
        std::size_t length{};
        std::size_t rstream{};
        rstream = stream->sputn(obuffer.data(), outLength);
        while(rstream != traits_type::eof() && (length += rstream) < outLength)
        {
            rstream = stream->sputn(obuffer.data() + length, outLength - length);
        }
    }
    if(ctx)
    {
        EVP_CIPHER_CTX_cleanup(ctx.get());
    }
}

int crypto::SymmetricSecureStream::underflow()
{
    std::size_t length{};
    std::size_t rstream{};
    int         outLength;

    rstream = stream->sgetn(ibuffer.data(), ibuffer.size());
    do {
        outLength = obuffer.size();
        while(rstream != traits_type::eof() && rstream != 0 && (length += rstream) < ibuffer.size())
        {
            rstream = stream->sgetn(ibuffer.data() + length, ibuffer.size() - length);
        }
        if(auto error = EVP_CipherUpdate(ctx.get(), reinterpret_cast<unsigned char *>(obuffer.data()), &outLength,
                                         reinterpret_cast<unsigned char *>(ibuffer.data()), length);
           error != 1)
        {
            throw std::runtime_error("int underflow");
            /*ERROR*/
        }
    } while(outLength == 0 && length == ibuffer.size());
    if(outLength == 0 && length != ibuffer.size())
    {
        outLength = obuffer.size();
        if(auto error = EVP_CipherFinal_ex(ctx.get(), reinterpret_cast<unsigned char *>(obuffer.data()), &outLength);
           error != 1)
        {
            return traits_type::eof();
            /*ERROR*/
        }
    }
    setg(obuffer.data(), obuffer.data(), obuffer.data() + outLength);

    return outLength == 0 ? traits_type::eof() : obuffer.front();
}

int crypto::SymmetricSecureStream::overflow(int ch)
{
    int outLength = obuffer.size();
    if(auto error = EVP_CipherUpdate(ctx.get(), reinterpret_cast<unsigned char *>(obuffer.data()), &outLength,
                                     reinterpret_cast<unsigned char *>(pbase()), std::distance(pbase(), pptr()));
       error <= 0)
    {
        throw std::runtime_error("int overflow");
        /*ERROR*/
    }

    setp(ibuffer.data(), ibuffer.data() + ibuffer.size());

    std::size_t length{};
    std::size_t rstream{};
    rstream = stream->sputn(obuffer.data(), outLength);
    while(rstream != traits_type::eof() && (length += rstream) < outLength)
    {
        rstream = stream->sputn(obuffer.data() + length, outLength - length);
    }

    if(ch != traits_type::eof())
    {
        *pptr() = traits_type::to_char_type(ch);
        pbump(1);
    }
    return ch;
}

int crypto::SymmetricSecureStream::sync()
{
    overflow();
    return stream->pubsync();
}
