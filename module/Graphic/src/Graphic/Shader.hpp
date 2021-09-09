#ifndef GAMEENGINEBONGO_GRAPHIC_SHADER_HPP
#define GAMEENGINEBONGO_GRAPHIC_SHADER_HPP

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/Shader.hpp>

namespace Graphic
{
    using OpenGL::Shader;
}// namespace Graphic

#else
#    error "invalid render platform"
#endif


#endif// GAMEENGINEBONGO_GRAPHIC_SHADER_HPP
