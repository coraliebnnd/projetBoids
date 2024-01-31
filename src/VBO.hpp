#pragma once

#include "p6/p6.h"

class VBO {
private:
    GLuint id;

public:
    VBO();

    void bind() const;
    void debind() const;

    ~VBO();
};
