#ifndef BONGOMOUNTAIN_SHADERGLPRIVATE_HPP
#define BONGOMOUNTAIN_SHADERGLPRIVATE_HPP

#include "opengl/Shader.hpp"
#include <render/private/ShaderPrivate.hpp>

class ShaderGLPrivate final : public render::detail::ShaderPrivate
{
public:
    void createFromStream(std::istream &code) override;
    void createFromSource(std::string_view code) override;
    void destroy() override;
    void bind() const override;
    int  getAttributeLocation(std::string_view name) const noexcept override;
    int  getUniformLocation(std::string_view name) const noexcept override;
    void setUniformValue(int i, const glm::mat4 &mat) noexcept override;
    void setUniformValue(int i, const glm::vec4 &vec) noexcept override;
    void setUniformValue(int i, const glm::vec3 &vec) noexcept override;
    void setUniformValue(int i, const glm::vec2 &vec) noexcept override;
    void setUniformValue(int i, int value) noexcept override;
    void setUniformValue(int i, float value) noexcept override;
    void setUniformValue(int i, double value) noexcept override;
    void setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept override;

private:
    opengl::Shader m_handle;
};


#endif// BONGOMOUNTAIN_SHADERGLPRIVATE_HPP
