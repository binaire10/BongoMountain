#include "Texture.hpp"
#include <fstream>
#include <stb_image.h>

Graphic::Texture Graphic::Texture::load_texture(std::string_view filename)
{
    try
    {
        if(*(filename.data() + filename.size()) == '\0')
            return load_texture(std::ifstream{ filename.data(), std::ios::binary | std::ios::in });
        else
        {
            BM_STACK_ARRAY(char, filename_fix, filename.size() + 1);
            std::copy(filename.begin(), filename.end(), filename_fix);
            filename_fix[filename.size()] = '\0';
            return load_texture(std::ifstream{ filename_fix, std::ios::binary | std::ios::in });
        }
    }
    catch(...)
    {
        std::throw_with_nested(std::runtime_error{ "fail to read file : " + std::string{ filename } });
    }
}

Graphic::Texture Graphic::Texture::load_texture(std::istream &stream)
{
    if(!stream)
        stream.exceptions(std::ios_base::failbit);

    int width, height, channels;

    static constexpr stbi_io_callbacks callbacks{
        [](void *user, char *data, int size) -> int {
            return reinterpret_cast<std::istream *>(user)->read(data, size).gcount();
        },
        [](void *user, int n) { reinterpret_cast<std::istream *>(user)->ignore(n); },
        [](void *user) -> int { return reinterpret_cast<std::istream *>(user)->eof(); }
    };
    static_assert(std::is_trivially_constructible_v<stbi_io_callbacks>);
    stbi_set_flip_vertically_on_load(1);
    stbi_uc               *data = stbi_load_from_callbacks(&callbacks, &stream, &width, &height, &channels, 0);
    Graphic::TextureFormat fmt  = TextureFormat::UNSPECIFIED;
    switch(channels)
    {
    case 4:
        fmt = TextureFormat::RGBA_8bit;
        break;
    case 3:
        fmt = TextureFormat::RGB_8bit;
        break;
    }
    if(data == nullptr)
        throw std::runtime_error{ "stbi failed to load image!" };

    if(fmt == TextureFormat::UNSPECIFIED)
        throw std::runtime_error{ "unsupported loaded image" };

    auto texture = load_texture(data, width, height, fmt);
    stbi_image_free(data);

    return texture;
}

Graphic::Texture
    Graphic::Texture::load_texture(const void *data, uint16_t width, uint16_t height, Graphic::TextureFormat fmt)
{
    return Graphic::Texture{ data, width, height, fmt };
}
