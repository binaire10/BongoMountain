add_library(common_graphic_option INTERFACE)
target_compile_definitions(common_graphic_option INTERFACE BM_SHARED_GRAPHIC)


add_subdirectory(extension)

add_library(BongoGraphic SHARED)
target_sources(BongoGraphic PRIVATE
        src/Graphic/config.hpp
        src/Graphic/Texture.cpp
        src/Graphic/BufferVector.hpp
        src/Graphic/detail/Traits.hpp
        src/Graphic/RenderType.hpp
        src/Graphic/RenderCommand.hpp
        src/Graphic/Context.hpp
        src/Graphic/EventWrapper.hpp
        src/Graphic/BufferLayout.h
        src/Graphic/BufferArray.hpp
        src/Graphic/GraphicModule.hpp
        src/Graphic/RenderSurface.hpp
        src/Graphic/GraphicApplication.hpp
        src/Graphic/Module.hpp
        src/Graphic/Module.cpp
        src/Event/Application/WindowCloseEvent.cpp
        src/Event/Application/WindowCloseEvent.h
        src/Event/Application/WindowResizeEvent.cpp
        src/Event/Application/WindowResizeEvent.h
        src/Event/Keyboard/KeyEvent.h
        src/Event/Keyboard/KeyEvent.cpp
        src/Event/Keyboard/KeyPressedEvent.h
        src/Event/Keyboard/KeyPressedEvent.cpp
        src/Event/Keyboard/KeyReleasedEvent.h
        src/Event/Keyboard/KeyReleasedEvent.cpp
        src/Event/Keyboard/KeyTypedEvent.h
        src/Event/Keyboard/KeyTypedEvent.cpp
        src/Event/Mouse/MouseButtonEvent.h
        src/Event/Mouse/MouseButtonEvent.cpp
        src/Event/Mouse/MouseButtonPressedEvent.h
        src/Event/Mouse/MouseButtonPressedEvent.cpp
        src/Event/Mouse/MouseButtonReleasedEvent.h
        src/Event/Mouse/MouseButtonReleasedEvent.cpp
        src/Event/Mouse/MouseScrolledEvent.h
        src/Event/Mouse/MouseScrolledEvent.cpp
        src/Event/Mouse/MouseMovedEvent.h
        src/Event/Mouse/MouseMovedEvent.cpp)

use_glfw(BongoGraphic)
use_glad(BongoGraphic)

target_link_libraries(BongoGraphic PUBLIC stbi BongoCore common_options)
target_compile_definitions(BongoGraphic PRIVATE BM_BUILD_GRAPHIC)
target_compile_definitions(BongoGraphic PUBLIC BM_SHARED_GRAPHIC)
target_include_directories(BongoGraphic PUBLIC src)