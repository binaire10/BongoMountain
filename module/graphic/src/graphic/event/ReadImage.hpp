#ifndef BONGOMOUNTAIN_READIMAGE_HPP
#define BONGOMOUNTAIN_READIMAGE_HPP

#include <variant>
#include <optional>
#include <fstream>

#include "../config.hpp"
#include <event/Event.hpp>
#include "../Image.hpp"

namespace graphic::event
{
    class BM_GRAPHIC_DCL ReadImage : public Event
    {
    public:
        EVENT_CLASS(Layer, ReadImage)

        explicit ReadImage(std::istream &stream)
            : m_stream(stream)
        {}

        [[nodiscard]] std::istream &getStream() { return m_stream; }

        inline void setImage(Image &&image) { ReadImage::m_image = std::move(image); }
        inline void setImage(const Image &image) { ReadImage::m_image = image; }

        [[nodiscard]] std::optional<Image> &getImage() { return m_image; }

    private:
        std::istream        &m_stream;
        std::optional<Image> m_image;
    };
}// namespace graphic::event

#endif// BONGOMOUNTAIN_READIMAGE_HPP
