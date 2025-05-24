
#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <vector>

#include "Image.hpp"
#include "ImageReader.hpp"

namespace graphic
{
    class BM_GRAPHIC_DCL ImageLoader
    {
    public:
        ~ImageLoader();
        std::optional<Image> load(std::string_view filename, std::streambuf *path);

    private:
        std::vector<std::unique_ptr<ImageReader>> readers;
    };
}// namespace graphic

#endif// IMAGELOADER_HPP
