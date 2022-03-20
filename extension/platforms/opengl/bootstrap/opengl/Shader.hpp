#ifndef BONGOMOUNTAIN_OPENGL_SHADER_HPP
#define BONGOMOUNTAIN_OPENGL_SHADER_HPP

#include <string_view>
#include <fstream>
#include <core/resource.hpp>
#include <core/memory.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/utility.hpp>

#include <glad/glad.h>

namespace opengl::detail
{
    struct ShaderTrait
    {
        static constexpr GLuint invalid_resource() noexcept { return 0; }

        static inline GLuint create() noexcept { return glCreateProgram(); }

        static inline void destroy(const GLuint &id) noexcept { glDeleteProgram(id); }
    };
}// namespace opengl::detail

namespace opengl
{
    template<template<typename, typename> typename ResourceOwnerT = core::resource>
    class ShaderGeneric
    {
    public:
        using program_ressource_type = ResourceOwnerT<GLuint, detail::ShaderTrait>;
        constexpr ShaderGeneric()    = default;
        explicit ShaderGeneric(program_ressource_type program)
            : program{ std::move(program) }
        {}
        template<template<typename, typename> typename ResourceOwnerY>
        ShaderGeneric(ShaderGeneric<ResourceOwnerY> &&move)
            : program{ move.release() }
        {}

        void destroy() { program.destroy(); }

        int getAttributeLocation(std::string_view name) const noexcept
        {
            return fixe_string(name,
                               [this](auto sname) { return glGetAttribLocation(program.getResource(), sname.data()); });
        }

        int getUniformLocation(std::string_view name) const noexcept
        {
            return fixe_string(
                name, [this](auto sname) { return glGetUniformLocation(program.getResource(), sname.data()); });
        }

        static void setUniformValue(int i, const glm::mat4 &mat) noexcept
        {
            glUniformMatrix4fv(i, 1, GL_FALSE, glm::value_ptr(mat));
        }

        static void setUniformValue(int i, const glm::vec4 &vec) noexcept { glUniform4fv(i, 1, glm::value_ptr(vec)); }

        static void setUniformValue(int i, const glm::vec3 &vec) noexcept { glUniform3fv(i, 1, glm::value_ptr(vec)); }


        static void setUniformValue(int i, const glm::vec2 &vec) noexcept { glUniform2fv(i, 1, glm::value_ptr(vec)); }

        static void setUniformValue(int i, int value) noexcept { glUniform1i(i, value); }

        static void setUniformValue(int i, float value) noexcept { glUniform1f(i, value); }

        static void setUniformValue(int i, double value) noexcept { glUniform1d(i, value); }

        static void setUniformArray(int i, const uint32_t *array, std::size_t count) noexcept
        {
            glUniform1uiv(i, count, array);
        }

        inline void bind() const noexcept { glUseProgram(program.getResource()); }

        program_ressource_type release() { return std::move(program); }

    private:
        program_ressource_type program;
    };

    using Shader       = ShaderGeneric<core::resource>;
    using SharedShader = ShaderGeneric<core::shared_resource>;

    Shader        createShaderFromCode(std::string_view codes);
    inline Shader createShaderFromStream(std::ifstream &stream) { return createShaderFromCode(core::readAll(stream)); }
    inline Shader createShaderFromStream(std::ifstream &&stream) { return createShaderFromStream(stream); }
    inline Shader createShaderFromStream(std::istream &stream) { return createShaderFromCode(core::readAll(stream)); }
    inline Shader createShaderFromStream(std::istream &&stream) { return createShaderFromStream(stream); }
}// namespace opengl


#endif// BONGOMOUNTAIN_OPENGL_SHADER_HPP
