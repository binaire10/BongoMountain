#include <istream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <core/Log.hpp>
#include <core/Platform.hpp>
#include <core/resource.hpp>
#include <event/EventDispatcher.hpp>
#include <graphic/ImageReader.hpp>
#include <core/Buffer.hpp>

#define UTILITY_PREFIX(...)    "[utility] " __VA_ARGS__
#define UTILITY_ERROR(...)     BM_CORE_ERROR(UTILITY_PREFIX(__VA_ARGS__))
#define UTILITY_INFO(...)      BM_CORE_INFO(UTILITY_PREFIX(__VA_ARGS__))
#define UTILITY_ASSERT(X, ...) BM_CUSTOM_ASSERT(UTILITY_ERROR, X, __VA_ARGS__)

namespace
{
    class StbLoader : public graphic::ImageReader
    {

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

    public:
        size_t               minProbeSize() const noexcept override { return 0; }
        size_t               maxProbeSize() const noexcept override { return 0; }
        graphic::ProbeStatus accept(std::string_view filename, std::span<const std::byte> probeData) const noexcept override
        {
            return graphic::Filename;
        }
        std::optional<graphic::Image> generate(std::span<const std::byte> probeData, std::streambuf *stream) override
        {
            using graphic::TextureFormat;
            using graphic::Image;

            struct StreamDecoder
            {
                std::streambuf                                     *stream;
                core::ByteTransaction<std::dynamic_extent, true> buffer;
            };

            int                                              width, height, channels;

            StreamDecoder ctx {
                stream,
                probeData
            };

            static constexpr stbi_io_callbacks callbacks{
                [](void *user, char *data, int size) -> int {
                    int byteRead = 0;
                    core::ByteTransaction<std::dynamic_extent> output(std::span<char>(data, size));
                    auto context = reinterpret_cast<StreamDecoder *>(user);
                    if(!context->buffer.full()) {
                        auto cbyte = output.write(context->buffer.position(), context->buffer.available_bytes());
                        context->buffer.accept(cbyte);
                        data += cbyte;
                        byteRead += cbyte;
                        size -= cbyte;
                    }
                    auto bcount = size > 0 ? context->stream->sgetn(data, size) : 0;

                    return byteRead + (bcount > 0 && byteRead > 0 ? 0 : bcount);
                },
                [](void *user, int n) {
                    auto context = reinterpret_cast<StreamDecoder *>(user);
                    if(!context->buffer.full()) {
                        auto byteRead = std::min(std::int64_t(n), std::int64_t(context->buffer.available_bytes()));
                        context->buffer.accept(byteRead);
                        n -= byteRead;
                    }
                    if(n != 0) {
                        context->stream->pubseekoff(n, std::ios::cur, std::ios::in);
                    }
                },
                [](void *user) -> int {
                    auto context = reinterpret_cast<StreamDecoder *>(user);
                    return context->buffer.full() && context->stream->in_avail() < 0;
                }
            };
            static_assert(std::is_trivially_constructible_v<stbi_io_callbacks>);
            stbi_set_flip_vertically_on_load(1);
            core::resource<stbi_uc *, stb_image_trait> stbi_data;
            stbi_data.create(&callbacks, &ctx, &width, &height, &channels, 0);
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
                return std::nullopt;

            if(fmt == TextureFormat::UNSPECIFIED)
                return std::nullopt;

            Image image(width, height, fmt, reinterpret_cast<const std::byte *>(stbi_data.getResource()));

            return image;
        }
    };
}// namespace


extern "C"
{
[[maybe_unused]] BM_EXPORT_DCL void initialize(core::RepositoryBindings &e, const nlohmann::json &configs)
{
    using namespace core;
    using module_binding = factory::define_type<StbLoader()>::bind<graphic::ImageReader>::make;
    e.register_factory<make_factory::add<module_binding>::build>();
    //    e.addLayer(std::make_unique<Module>());
}
}
