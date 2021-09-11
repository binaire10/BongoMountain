#include "Shader.hpp"
#include <Core/Log.hpp>
#include <iterator>
#include <map>

namespace
{
    struct ShaderTrait
    {
        static constexpr GLuint invalid_resource() noexcept { return 0; }

        static inline GLuint create(GLenum type) noexcept { return glCreateShader(type); }

        static inline void destroy(const GLuint &id) noexcept { glDeleteShader(id); }
    };
    using ShaderResource = Core::resource<GLuint, ShaderTrait>;

    void compile(GLuint id, const std::pair<std::size_t, std::string_view> &input)
    {
        auto &[line, code] = input;
        auto versionPos    = code.find("#version");

        std::string define = "#line " + std::to_string(line + 1) + "\n";

        if(versionPos != std::string_view::npos)
        {
            auto          end_line   = code.find('\n') != std::string_view::npos ? '\n' : '\r';
            auto          endLinePos = code.find(end_line, versionPos);
            const GLchar *src[3]{ code.data() + versionPos, define.c_str(), code.data() + endLinePos + 1 };
            GLint         size[3]{ static_cast<GLint>(endLinePos - versionPos + 1), static_cast<GLint>(define.size()),
                           static_cast<GLint>(code.size() - endLinePos - 1) };
            glShaderSource(id, 3, src, size);
        }
        else
        {
            const GLchar *src[2]{ define.c_str(), code.data() };
            GLint         size[2]{ static_cast<GLint>(define.size()), static_cast<GLint>(code.size()) };
            glShaderSource(id, 2, src, size);
        }

        glCompileShader(id);
        GLint isCompiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            BM_STACK_ARRAY(GLchar, infoLog, maxLength);

            glGetShaderInfoLog(id, maxLength, &maxLength, infoLog);

            glDeleteShader(id);

            BM_CORE_ERROR("{0}", infoLog);

            throw std::runtime_error{ "Shader compilation failure" };
        }
    }

    GLenum shaderTypeFromString(std::string_view type)
    {
        using namespace std::string_view_literals;
        if(type == "vertex"sv)
            return GL_VERTEX_SHADER;
        if(type == "fragment"sv || type == "pixel"sv)
            return GL_FRAGMENT_SHADER;
        throw std::runtime_error{ "Unknown shader type!" };
    }

    std::map<GLenum, std::pair<std::size_t, std::string_view>> split_shader(std::string_view code)
    {
        using namespace std::string_view_literals;
        static constexpr auto typeToken = "#type"sv;

        auto end_line = code.find('\n') != std::string_view::npos ? '\n' : '\r';

        std::map<GLenum, std::pair<std::size_t, std::string_view>> shaders;
        auto                                                       offset = code.find(typeToken);
        while(offset != std::string_view::npos)
        {
            auto startType = code.find_first_not_of(' ', offset + typeToken.size());
            //            BM_CORE_ASSERT(startType != std::string_view ::npos, "Syntax error");
            auto eol = code.find_first_of("\r\n", startType);
            //            BM_CORE_ASSERT(eol != std::string_view ::npos, "Syntax error");
            std::string_view type = code.substr(startType, eol - startType);

            offset                = code.find(typeToken, eol);
            std::string_view data = offset == std::string::npos ? code.substr(eol) : code.substr(eol, offset - eol);
            shaders.emplace(shaderTypeFromString(type),
                            std::make_pair(std::count(code.begin(), code.begin() + eol, end_line) + 1, data));
        }
        return shaders;
    }
}// namespace

OpenGL::Shader OpenGL::Shader::createFromCode(std::string_view codes)
{
    const auto        splitCode = split_shader(codes);
    const std::size_t count     = splitCode.size();
    BM_STACK_ARRAY(ShaderResource, shaders, count);
    unsigned i{};
    for(auto &[type, src] : splitCode)
    {
        ShaderResource &shader = shaders[i++];
        shader.create(type);
        compile(shader.getResource(), src);
    }
    GLuint id = glCreateProgram();
    Shader shader{ Shader::ProgramResource{ id } };
    std::for_each(shaders, shaders + count, [id](const ShaderResource &s) { glAttachShader(id, s.getResource()); });
    glLinkProgram(id);

    GLint isLinked = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &isLinked);

    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        BM_STACK_ARRAY(GLchar, infoLog, maxLength + 1);

        glGetProgramInfoLog(id, maxLength, &maxLength, infoLog);
        infoLog[maxLength] = '\0';

        BM_CORE_ERROR("{0}", infoLog);
        std::for_each(shaders, shaders + count, [id](const ShaderResource &s) { glDetachShader(id, s.getResource()); });
        throw std::runtime_error{ "Shader link failure!" };
    }
    std::for_each(shaders, shaders + count, [id](const ShaderResource &s) { glDetachShader(id, s.getResource()); });

    return shader;
}

OpenGL::Shader OpenGL::Shader::createFromStream(std::ifstream &stream)
{
    stream.exceptions(std::ios::failbit);
    stream.seekg(0, std::ios::end);
    auto        count = stream.tellg();
    std::string buffer(count, '\0');
    stream.seekg(0, std::ios::beg);
    stream.read(buffer.data(), count);
    return createFromCode(buffer);
}

OpenGL::Shader OpenGL::Shader::createFromStream(std::istream &stream)
{
    stream.exceptions(std::ios::failbit);
    std::string buffer(std::istreambuf_iterator<char>{ stream }, std::istreambuf_iterator<char>{});
    return createFromCode(buffer);
}
