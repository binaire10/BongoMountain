#include "Image.hpp"

graphic::Image graphic::Image::subImage(std::size_t offsetX, std::size_t offsetY, std::size_t width, std::size_t height)
{
    std::vector<std::byte> data(width * height);
    for(std::size_t i{}; i < height; ++i)
    {
        std::size_t offset = (i + offsetY) * m_width + offsetX;
        std::copy(m_data.begin() + offset * sizeofPixel(m_format),
                  m_data.begin() + (offset + width) * sizeofPixel(m_format),
                  data.begin() + (i * width) * sizeofPixel(m_format));
    }
    return Image{ width, height, m_format, std::move(data) };
}
