#pragma once

#include "p6/p6.h"

class Texture {
private:
    GLuint id;

public:
    Texture();

    void bind() const;
    void set(std::string image) const;
    void send(GLint uTexture) const;
    void debind() const;

    ~Texture();
};