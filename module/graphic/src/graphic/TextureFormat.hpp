#ifndef BONGOMOUNTAIN_TEXTUREFORMAT_HPP
#define BONGOMOUNTAIN_TEXTUREFORMAT_HPP

#include <cstdint>
#include <stdexcept>

namespace graphic
{
    enum class TextureFormat { RGB_8bit, RGB_16bit, RGB_32bit, RGBA_8bit, RGBA_16bit, RGBA_32bit, UNSPECIFIED };

    constexpr std::size_t sizeofPixel(TextureFormat format) {
        switch(format)
        {
        case TextureFormat::RGB_8bit:
            return 3 * sizeof(uint8_t);
        case TextureFormat::RGB_16bit:
            return 3 * sizeof(uint16_t);
        case TextureFormat::RGB_32bit:
            return 3 * sizeof(uint32_t);
        case TextureFormat::RGBA_8bit:
            return 4 * sizeof(uint8_t);
        case TextureFormat::RGBA_16bit:
            return 4 * sizeof(uint16_t);
        case TextureFormat::RGBA_32bit:
            return 4 * sizeof(uint32_t);
        }
        throw std::runtime_error{"undefined size for format"};
    }
}

#endif// BONGOMOUNTAIN_TEXTUREFORMAT_HPP
