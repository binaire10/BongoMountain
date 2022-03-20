#include "Image.hpp"
#include "event/ReadImage.hpp"

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

graphic::Image graphic::Image::createFromStream(core::Platform &platform, std::istream &stream)
{
    graphic::event::ReadImage query{ stream };
    platform.dispatchEvent(query);
    if(auto &image = query.getImage())
        return std::move(*query.getImage());
    throw std::runtime_error{"no image in the query generate."};
}
