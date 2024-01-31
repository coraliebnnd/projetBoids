#include "VBO.hpp"

VBO::VBO()
{
    glGenBuffers(1, &id);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::debind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}