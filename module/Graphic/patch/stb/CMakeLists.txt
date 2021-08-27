add_library(stbi "")
target_include_directories(stbi PUBLIC .)

FILE(WRITE stbi.cpp
        "#include \"stb_image.h\"\n"
        )

target_sources(stbi PUBLIC
        stb_image.h
        PRIVATE
        stbi.cpp
        )
target_compile_definitions(stbi PRIVATE STB_IMAGE_IMPLEMENTATION)
