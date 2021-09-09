#ifndef GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP
#define GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/BufferObject.hpp>

namespace Graphic
{
    using OpenGL::copy;

    using OpenGL::VertexBuffer;
    using OpenGL::VertexBufferStorage;
    using OpenGL::IndexBuffer;
    using OpenGL::IndexBufferStorage;

    using OpenGL::SharedVertexBuffer;
    using OpenGL::SharedIndexBuffer;
    using OpenGL::SharedIndexBufferStorage;
    using OpenGL::SharedVertexBufferStorage;
}// namespace Graphic

#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_GRAPHIC_BUFFEROBJECT_HPP
