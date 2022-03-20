#include "ShaderGLPrivate.hpp"

void ShaderGLPrivate::createFromStream(std::istream &code) { m_handle = opengl::createShaderFromStream(code); }

void ShaderGLPrivate::createFromSource(std::string_view code) { m_handle = opengl::createShaderFromCode(code); }

void ShaderGLPrivate::destroy() { m_handle.destroy(); }

void ShaderGLPrivate::bind() const { m_handle.bind(); }

int ShaderGLPrivate::getAttributeLocation(std::string_view name) const noexcept
{
    return m_handle.getAttributeLocation(name);
}

int ShaderGLPrivate::getUniformLocation(std::string_view name) const noexcept
{
    return m_handle.getUniformLocation(name);
}

void ShaderGLPrivate::setUniformValue(int i, const glm::mat4 &mat) noexcept { m_handle.setUniformValue(i, mat); }

void ShaderGLPrivate::setUniformValue(int i, const glm::vec4 &vec) noexcept { m_handle.setUniformValue(i, vec); }

void ShaderGLPrivate::setUniformValue(int i, const glm::vec3 &vec) noexcept { m_handle.setUniformValue(i, vec); }

void ShaderGLPrivate::setUniformValue(int i, const glm::vec2 &vec) noexcept { m_handle.setUniformValue(i, vec); }

void ShaderGLPrivate::setUniformValue(int i, int value) noexcept { m_handle.setUniformValue(i, value); }

void ShaderGLPrivate::setUniformValue(int i, float value) noexcept { m_handle.setUniformValue(i, value); }

void ShaderGLPrivate::setUniformValue(int i, double value) noexcept { m_handle.setUniformValue(i, value); }

void ShaderGLPrivate::setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept
{
    m_handle.setUniformArray(i, array, count);
}