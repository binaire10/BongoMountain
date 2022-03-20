#ifndef BONGOMOUNTAIN_SHADER_HPP
#define BONGOMOUNTAIN_SHADER_HPP

#include <memory>
#include <istream>
#include <string_view>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"

namespace render::detail
{
    class ShaderPrivate;
}

namespace render
{
    class BM_GRAPHIC_DCL Shader final : public RenderElement
    {
    public:
        Shader(RenderDevice *owner, std::unique_ptr<detail::ShaderPrivate> &&handler);
        ~Shader() override;

        Shader(Shader &&) noexcept = default;
        Shader &operator=(Shader &&) noexcept = default;

        void        createFromStream(std::istream &code);
        inline void createFromStream(std::istream &&code) { createFromStream(code); }

        void createFromSource(std::string_view code);

        void destroy();

        void bind() const;

        int getAttributeLocation(std::string_view name) const noexcept;

        int getUniformLocation(std::string_view name) const noexcept;

        void setUniformValue(int i, const glm::mat4 &mat) noexcept;

        void setUniformValue(int i, const glm::vec4 &vec) noexcept;

        void setUniformValue(int i, const glm::vec3 &vec) noexcept;

        void setUniformValue(int i, const glm::vec2 &vec) noexcept;

        void setUniformValue(int i, int value) noexcept;

        void setUniformValue(int i, float value) noexcept;

        void setUniformValue(int i, double value) noexcept;

        void setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept;

        [[nodiscard]] constexpr const std::unique_ptr<detail::ShaderPrivate> &getPrivate() const { return m_handler; }

    private:
        std::unique_ptr<detail::ShaderPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_SHADER_HPP
