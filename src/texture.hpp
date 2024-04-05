#pragma once

#include "p6/p6.h"

class Texture {
private:
    GLuint id;

public:
    Texture();

    void bind() const;
    void set(img::Image image) const;
    void debind() const;

    ~Texture();
};