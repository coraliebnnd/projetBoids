#include "object.hpp"
#include <iostream>
#include <string>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../src-common/tiny_obj_loader.h"

Object::Object()
    : m_Program(p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/tex3D.fs.glsl"))
{
    uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
    uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
    uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
};

void Object::setInputfile(std::string filename)
{
    inputfile = "../assets/models/" + filename + ".obj";
};

void Object::load()
{
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

    if (!warn.empty())
    {
        std::cout << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        exit(1);
    }

    // Loop over shapes
    for (const auto& shape : shapes)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                // access to vertex
                glimac::ShapeVertex newVertex = glimac::ShapeVertex{

                    // POSITION
                    glm::vec3(
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 0]),
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 1]),
                        tinyobj::real_t(attrib.vertices[3 * size_t(idx.vertex_index) + 2])
                    ),
                    // NORMAL
                    glm::vec3(
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 0]), // nx
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 1]), // ny
                        tinyobj::real_t(attrib.normals[3 * size_t(idx.normal_index) + 2])  // nz
                    ),
                    // TEXTURE_COORDINATES
                    glm::vec2(
                        tinyobj::real_t(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]), // tx
                        tinyobj::real_t(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1])  // ty
                    )
                };

                vertices.push_back(newVertex);
            }
            index_offset += fv;
        }
    }
};

void Object::matrices(glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec<DIMENSION, float> position)
{
    // matrix model
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix           = glm::scale(modelMatrix, glm::vec3(0.01, 0.01, 0.01));

    // matrix model-vue
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // matrix MVP
    glm::mat4 MVPMatrixBoid = projMatrix * modelViewMatrix;

    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrixBoid));
};

void Object::draw()
{
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
};

std::vector<glimac::ShapeVertex> Object::getVertices()
{
    return vertices;
};

GLint Object::getUTexture()
{
    return uTexture;
}

void Object::use()
{
    m_Program.use();
}