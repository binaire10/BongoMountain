#ifndef BONGOMOUNTAIN_CONFIG_HPP
#define BONGOMOUNTAIN_CONFIG_HPP

#if defined(_WIN32) || defined(__CYGWIN__)

#    ifdef __GNUC__
#        define BM_IMPORT_DCL __attribute__((dllimport))
#        define BM_EXPORT_DCL __attribute__((dllexport))
#        define BM_LOCAL_DCL
#    else
#        define BM_IMPORT_DCL __declspec(dllimport)
#        define BM_EXPORT_DCL __declspec(dllexport)
#        define BM_LOCAL_DCL
#    endif
#else
#    define BM_IMPORT_DCL __attribute__((visibility("default")))
#    define BM_EXPORT_DCL __attribute__((visibility("default")))
#    define BM_LOCAL_DCL  __attribute__((visibility("hidden")))
#endif


#ifdef BM_SHARED_CORE
#    ifdef BM_BUILD_CORE
#        define BM_CORE_DCL BM_EXPORT_DCL
#    else
#        define BM_CORE_DCL BM_IMPORT_DCL
#    endif
#else
#    define BM_CORE_DCL
#endif


#endif// BONGOMOUNTAIN_CONFIG_HPP
