#ifndef GAMEENGINEBONGO_RENDER_CONFIG_HPP
#define GAMEENGINEBONGO_RENDER_CONFIG_HPP

#include <Core/config.hpp>

#ifdef BM_SHARED_RENDER
#    ifdef BM_BUILD_RENDER
#        define BM_RENDER_DCL BM_EXPORT_DCL
#    else
#        define BM_RENDER_DCL BM_IMPORT_DCL
#    endif
#else
#    define BM_RENDER_DCL
#endif

#endif// GAMEENGINEBONGO_RENDER_CONFIG_HPP
