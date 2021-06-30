#ifndef GAMEENGINEBONGO_SHADER_HPP
#define GAMEENGINEBONGO_SHADER_HPP

#include <string_view>
#include <fstream>
#include <Core/Ressource.hpp>
#include <Core/memory.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.hpp"

#include <Graphic/config.hpp>

namespace OpenGL
{
    class Shader
    {
        struct ShaderTrait
        {
            static constexpr GLuint invalid_resource() noexcept { return 0; }

            static inline GLuint create() noexcept { return glCreateProgram(); }

            static inline void destroy(const GLuint &id) noexcept { glDeleteProgram(id); }
        };

    public:
        using ProgramResource = Resource<GLuint, ShaderTrait>;
        explicit inline Shader(ProgramResource program)
            : program{ std::move(program) }
        {}

        inline int getAttributeLocation(std::string_view name) noexcept
        {
            if(name[name.size()] != '\0')
            {
                BM_STACK_ARRAY(GLchar, sname, name.size());
                std::copy(name.begin(), name.end(), sname);
                return glGetAttribLocation(program.getResource(), sname);
            }
            return glGetAttribLocation(program.getResource(), name.data());
        }

        inline int getUniformLocation(std::string_view name) noexcept
        {
            if(*(name.data() + name.size()) != '\0')
            {
                BM_STACK_ARRAY(GLchar, sname, name.size());
                std::copy(name.begin(), name.end(), sname);
                return glGetUniformLocation(program.getResource(), sname);
            }
            return glGetUniformLocation(program.getResource(), name.data());
        }

        static inline void setUniformValue(int i, const glm::mat4 &mat) noexcept
        {
            glUniformMatrix4fv(i, 1, GL_FALSE, glm::value_ptr(mat));
        }

        static inline void setUniformValue(int i, const glm::vec4 &vec) noexcept
        {
            glUniform4fv(i, 1, glm::value_ptr(vec));
        }

        static inline void setUniformValue(int i, const glm::vec3 &vec) noexcept
        {
            glUniform3fv(i, 1, glm::value_ptr(vec));
        }


        static inline void setUniformValue(int i, const glm::vec2 &vec) noexcept
        {
            glUniform2fv(i, 1, glm::value_ptr(vec));
        }

        inline void                  bind() const noexcept { glUseProgram(program.getResource()); }
        BM_GRAPHIC_DCL static Shader createFromStream(std::ifstream &stream);
        inline static Shader         createFromStream(std::ifstream &&stream) { return createFromStream(stream); }
        BM_GRAPHIC_DCL static Shader createFromStream(std::istream &stream);
        inline static Shader         createFromStream(std::istream &&stream) { return createFromStream(stream); }
        BM_GRAPHIC_DCL static Shader createFromCode(std::string_view codes);

    private:
        ProgramResource program;
    };
}// namespace OpenGL


#endif// GAMEENGINEBONGO_SHADER_HPP
