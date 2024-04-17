#pragma once

#include "../src-common/glimac/common.hpp"
#include "p6/p6.h"

class VBO {
private:
    GLuint id;

public:
    VBO();

    void bind() const;
    void sendData(std::vector<glimac::ShapeVertex> vertices);
    void debind() const;

    ~VBO();
};
