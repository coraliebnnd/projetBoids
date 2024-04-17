#include "VBO.hpp"
#include "../src-common/glimac/common.hpp"

VBO::VBO()
{
    glGenBuffers(1, &id);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::sendData(std::vector<glimac::ShapeVertex> vertices)
{
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::debind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}