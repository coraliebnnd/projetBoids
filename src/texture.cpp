#include "texture.hpp"

Texture::Texture()
{
    glGenTextures(1, &id);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::set(std::string filename) const
{
    const img::Image image = p6::load_image_buffer("../assets/textures/" + filename + ".png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::send(GLint uTexture) const
{
    glUniform1i(uTexture, 0);
}

void Texture::debind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}