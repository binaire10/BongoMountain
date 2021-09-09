
#ifndef GAMEENGINEBONGO_RENDERCOMMAND_HPP
#define GAMEENGINEBONGO_RENDERCOMMAND_HPP

#include "Texture.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "BufferObject.hpp"

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/RenderCommand.hpp>

#    include <OpenGL/BufferObject.hpp>
#    include <OpenGL/Shader.hpp>
#    include <OpenGL/VertexArray.hpp>
#    include <OpenGL/Texture.hpp>
#    include <Core/PointerLikeStackVariable.hpp>

namespace Graphic
{
    using OpenGL::RenderCommand;
}// namespace Graphic

#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_RENDERCOMMAND_HPP
