#include "Shader.hpp"
#include "private/ShaderPrivate.hpp"

render::Shader::Shader(render::RenderDevice *owner, std::unique_ptr<detail::ShaderPrivate> &&handler)
    : RenderElement(owner)
    , m_handler(std::move(handler))
{
    BM_CORE_ASSERT(m_handler, "Shader handler is require");
}

render::Shader::~Shader() = default;

void render::Shader::createFromStream(std::istream &code) { m_handler->createFromStream(code); }

void render::Shader::createFromSource(std::string_view code) { m_handler->createFromSource(code); }

void render::Shader::destroy() { m_handler->destroy(); }

void render::Shader::bind() const { m_handler->bind(); }

int render::Shader::getAttributeLocation(std::string_view name) const noexcept
{
    return m_handler->getAttributeLocation(name);
}

int render::Shader::getUniformLocation(std::string_view name) const noexcept
{
    return m_handler->getUniformLocation(name);
}

void render::Shader::setUniformValue(int i, const glm::mat4 &mat) noexcept { m_handler->setUniformValue(i, mat); }

void render::Shader::setUniformValue(int i, const glm::vec4 &vec) noexcept { m_handler->setUniformValue(i, vec); }

void render::Shader::setUniformValue(int i, const glm::vec3 &vec) noexcept { m_handler->setUniformValue(i, vec); }

void render::Shader::setUniformValue(int i, const glm::vec2 &vec) noexcept { m_handler->setUniformValue(i, vec); }

void render::Shader::setUniformValue(int i, int value) noexcept { m_handler->setUniformValue(i, value); }

void render::Shader::setUniformValue(int i, float value) noexcept { m_handler->setUniformValue(i, value); }

void render::Shader::setUniformValue(int i, double value) noexcept { m_handler->setUniformValue(i, value); }
void render::Shader::setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept
{
    m_handler->setUniformArray(i, array, count);
}
