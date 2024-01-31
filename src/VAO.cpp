#include "VAO.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::bind() const
{
    glBindVertexArray(id);
}

void VAO::debind() const
{
    glBindVertexArray(0);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &id);
}