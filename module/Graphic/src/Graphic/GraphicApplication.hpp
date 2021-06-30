#ifndef GAMEENGINEBONGO_GRAPHICAPPLICATION_HPP
#define GAMEENGINEBONGO_GRAPHICAPPLICATION_HPP

#if defined(BM_PLATFORM_GLFW)
#    include <GLFW/GraphicApplication.hpp>
#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_GRAPHICAPPLICATION_HPP
