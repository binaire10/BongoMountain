#ifndef GAMEENGINEBONGO_GRAPHIC_CONFIG_HPP
#define GAMEENGINEBONGO_GRAPHIC_CONFIG_HPP

#include <Core/config.hpp>

#ifdef BM_SHARED_GRAPHIC
#    ifdef BM_BUILD_GRAPHIC
#        define BM_GRAPHIC_DCL BM_EXPORT_DCL
#    else
#        define BM_GRAPHIC_DCL BM_IMPORT_DCL
#    endif
#else
#    define BM_GRAPHIC_DCL
#endif

namespace Graphic
{
    class RenderSurface;
}

#endif// GAMEENGINEBONGO_GRAPHIC_CONFIG_HPP
