#ifndef GAMEENGINEBONGO_RENDERSURFACE_HPP
#define GAMEENGINEBONGO_RENDERSURFACE_HPP

#if defined(BM_PLATFORM_GLFW)
#    include <GLFW/RenderSurface.hpp>
#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_RENDERSURFACE_HPP
