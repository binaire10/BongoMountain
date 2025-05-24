#ifndef IMAGEREADER_HPP
#define IMAGEREADER_HPP

#include <cstdint>
#include <string>
#include <streambuf>
#include <optional>
#include "Image.hpp"

namespace graphic
{
    enum ProbeStatus { NotRecognized, Filename, Header, Data };

    class ImageReader
    {
    public:
        virtual std::size_t          minProbeSize() const noexcept                                                = 0;
        virtual std::size_t          maxProbeSize() const noexcept                                                = 0;
        virtual ProbeStatus          accept(std::string_view filename, std::span<const std::byte> probeData) const noexcept = 0;
        virtual std::optional<Image> generate(std::span<const std::byte> probeData, std::streambuf *stream)       = 0;
    };

}// namespace graphic

#endif// IMAGEREADER_HPP
