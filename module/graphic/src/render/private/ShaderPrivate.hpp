#ifndef BONGOMOUNTAIN_SHADERPRIVATE_HPP
#define BONGOMOUNTAIN_SHADERPRIVATE_HPP

#include <string_view>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace render::detail
{
    class ShaderPrivate
    {
    public:
        virtual ~ShaderPrivate() = default;

        virtual void createFromStream(std::istream &code)    = 0;
        virtual void createFromSource(std::string_view code) = 0;

        virtual void destroy() = 0;

        virtual void bind() const = 0;

        virtual int getAttributeLocation(std::string_view name) const noexcept = 0;

        virtual int getUniformLocation(std::string_view name) const noexcept = 0;

        virtual void setUniformValue(int i, const glm::mat4 &mat) noexcept = 0;
        virtual void setUniformValue(int i, const glm::vec4 &vec) noexcept = 0;
        virtual void setUniformValue(int i, const glm::vec3 &vec) noexcept = 0;
        virtual void setUniformValue(int i, const glm::vec2 &vec) noexcept = 0;
        virtual void setUniformValue(int i, int value) noexcept            = 0;
        virtual void setUniformValue(int i, float value) noexcept          = 0;
        virtual void setUniformValue(int i, double value) noexcept         = 0;

        virtual void setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept = 0;
    };
}// namespace render::detail

#endif// BONGOMOUNTAIN_SHADERPRIVATE_HPP
