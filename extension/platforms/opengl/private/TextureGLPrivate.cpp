#include "TextureGLPrivate.hpp"

using opengl::Texture;

void TextureGLPrivate::create(const graphic::ImageView &image) { texture = Texture::create(image); }

void TextureGLPrivate::bind() const { texture.bind(); }

void TextureGLPrivate::bind(unsigned id) const { texture.bind(id); }

void TextureGLPrivate::overwrite(const graphic::ImageView &image) { texture.overwrite(image); }
