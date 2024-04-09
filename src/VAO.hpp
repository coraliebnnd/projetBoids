#pragma once

#include "p6/p6.h"

class VAO {
private:
    GLuint id;

public:
    VAO();

    void bind() const;
    void set() const;
    void debind() const;

    ~VAO();
};