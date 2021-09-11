#ifndef GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP
#define GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/BufferObject.hpp>

namespace Graphic
{
    using OpenGL::VertexBuffer;
    using OpenGL::IndexBuffer;

    using OpenGL::SharedVertexBuffer;
    using OpenGL::SharedIndexBuffer;
}// namespace Graphic

#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP
