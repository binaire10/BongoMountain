#include <istream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <core/Log.hpp>
#include <core/Platform.hpp>
#include <core/resource.hpp>
#include <event/EventDispatcher.hpp>
#include <graphic/event/ReadImage.hpp>

#define UTILITY_PREFIX(...)    "[utility] " __VA_ARGS__
#define UTILITY_ERROR(...)     BM_CORE_ERROR(UTILITY_PREFIX(__VA_ARGS__))
#define UTILITY_INFO(...)      BM_CORE_INFO(UTILITY_PREFIX(__VA_ARGS__))
#define UTILITY_ASSERT(X, ...) BM_CUSTOM_ASSERT(UTILITY_ERROR, X, __VA_ARGS__)

namespace
{
    class Module : public core::Layer
    {
    public:
        void onAttach() override { UTILITY_INFO("Attach"); }
        void onDetach() override { UTILITY_INFO("Detach"); }
        void onBegin() override {}
        void onEnd() override {}
        void onUpdate() override {}
        void onEvent(Event &event) override
        {
            using graphic::event::ReadImage;
            EventDispatcher dispatcher{ event };
            dispatcher.dispatch<ReadImage>([](ReadImage &event) { event.setImage(Module::load(event.getStream())); });
        }

    private:
        struct stb_image_trait
        {
            template<typename... ArgT>
            static stbi_uc *create(ArgT... arg)
            {
                return stbi_load_from_callbacks(std::forward<ArgT>(arg)...);
            }

            static void destroy(stbi_uc *ptr) { stbi_image_free(ptr); }

            static consteval stbi_uc *invalid_resource() { return nullptr; }
        };

        static graphic::Image load(std::istream &stream)
        {
            using graphic::TextureFormat;
            using graphic::Image;

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
            core::resource<stbi_uc *, stb_image_trait> stbi_data;
            stbi_data.create(&callbacks, &stream, &width, &height, &channels, 0);
            graphic::TextureFormat fmt = TextureFormat::UNSPECIFIED;
            switch(channels)
            {
            case 4:
                fmt = graphic::TextureFormat::RGBA_8bit;
                break;
            case 3:
                fmt = TextureFormat::RGB_8bit;
                break;
            }
            if(!stbi_data)
                throw std::runtime_error{ "stbi failed to load image!" };

            if(fmt == TextureFormat::UNSPECIFIED)
                throw std::runtime_error{ "unsupported loaded image" };

            Image image(width, height, fmt, reinterpret_cast<const std::byte *>(stbi_data.getResource()));

            return image;
        }
    };
}// namespace


extern "C" {
BM_EXPORT_DCL void initialize(core::ExtensionManager &e, const nlohmann::json &configs)
{
    e.addLayer(std::make_unique<Module>());
}
}
