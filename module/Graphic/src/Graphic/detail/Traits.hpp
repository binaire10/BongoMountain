#ifndef GAMEENGINEBONGO_TRAITS_HPP
#define GAMEENGINEBONGO_TRAITS_HPP

#if defined(BM_PLATFORM_OPENGL)
#    include <OpenGL/detail/Traits.hpp>

namespace Graphic::detail {
    using OpenGL::detail::container_trait;
}

#endif


#endif// GAMEENGINEBONGO_TRAITS_HPP
