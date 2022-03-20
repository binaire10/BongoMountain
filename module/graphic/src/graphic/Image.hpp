#ifndef BONGOMOUNTAIN_IMAGE_HPP
#define BONGOMOUNTAIN_IMAGE_HPP

#include <bit>
#include <vector>

#include "config.hpp"
#include "TextureFormat.hpp"
#include <core/Platform.hpp>

namespace graphic
{
    class ImageView
    {
    public:
        constexpr ImageView(size_t width, size_t height, TextureFormat format, std::byte *data)
            : m_width(width)
            , m_height(height)
            , m_data(data)
            , m_format(format)
        {}

        [[nodiscard]] constexpr size_t getWidth() const { return m_width; }
        [[nodiscard]] constexpr size_t getHeight() const { return m_height; }

        [[nodiscard]] constexpr TextureFormat getFormat() const { return m_format; }

        [[nodiscard]] constexpr const std::byte *getData() const { return m_data; }

        constexpr bool empty() const { return m_width * m_height == 0; }

    private:
        std::size_t   m_width;
        std::size_t   m_height;
        std::byte    *m_data;
        TextureFormat m_format;
    };

    class Image
    {
    public:
        inline Image()
            : m_width(0)
            , m_height(0)
            , m_format(TextureFormat::UNSPECIFIED)
        {}

        inline explicit Image(const ImageView &image)
            : m_width(image.getWidth())
            , m_height(image.getHeight())
            , m_data(image.getData(), image.getData() + m_width * m_height)
            , m_format(image.getFormat())
        {}

        inline Image(size_t width, size_t height, TextureFormat format, const std::byte *data)
            : m_width(width)
            , m_height(height)
            , m_data(data, data + width * height * sizeofPixel(format))
            , m_format(format)
        {}

        inline Image(size_t width, size_t height, TextureFormat format, std::vector<std::byte> data)
            : m_width(width)
            , m_height(height)
            , m_data(std::move(data))
            , m_format(format)
        {}

        [[nodiscard]] operator ImageView() { return ImageView{ m_width, m_height, m_format, m_data.data() }; }

        [[nodiscard]] inline size_t getWidth() const { return m_width; }
        [[nodiscard]] inline size_t getHeight() const { return m_height; }

        [[nodiscard]] inline TextureFormat getFormat() const { return m_format; }

        [[nodiscard]] inline const std::byte *getData() const { return m_data.data(); }

        [[nodiscard]] BM_GRAPHIC_DCL Image subImage(std::size_t offsetX,
                                                    std::size_t offsetY,
                                                    std::size_t width,
                                                    std::size_t height);

        [[nodiscard]] BM_GRAPHIC_DCL static Image createFromStream(core::Platform &platform, std::istream &stream);

        [[nodiscard]] inline static Image createFromStream(core::Platform &platform, std::istream &&stream)
        {
            return createFromStream(platform, stream);
        }

        [[nodiscard]] inline bool empty() const { return m_width * m_height == 0; }

    private:
        std::size_t            m_width;
        std::size_t            m_height;
        std::vector<std::byte> m_data;
        TextureFormat          m_format;
    };
}// namespace graphic

#endif// BONGOMOUNTAIN_IMAGE_HPP
