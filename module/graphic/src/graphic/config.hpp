#ifndef BONGOMOUNTAIN_GRAPHIC_CONFIG_HPP
#define BONGOMOUNTAIN_GRAPHIC_CONFIG_HPP

#include <core/config.hpp>

#ifdef BM_SHARED_GRAPHIC
#    ifdef BM_BUILD_GRAPHIC
#        define BM_GRAPHIC_DCL BM_EXPORT_DCL
#    else
#        define BM_GRAPHIC_DCL BM_IMPORT_DCL
#    endif
#else
#    define BM_GRAPHIC_DCL
#endif

#endif// BONGOMOUNTAIN_GRAPHIC_CONFIG_HPP
