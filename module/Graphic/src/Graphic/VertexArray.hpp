#ifndef GAMEENGINEBONGO_GRAPHIC_VERTEXARRAY_HPP
#define GAMEENGINEBONGO_GRAPHIC_VERTEXARRAY_HPP

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/RenderCommand.hpp>

#    include <OpenGL/BufferObject.hpp>
#    include <OpenGL/Shader.hpp>
#    include <OpenGL/VertexArray.hpp>
#    include <OpenGL/Texture.hpp>
#    include <Core/PointerLikeStackVariable.hpp>

namespace Graphic
{
    using OpenGL::VertexArray;
}// namespace Graphic

#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_GRAPHIC_VERTEXARRAY_HPP
