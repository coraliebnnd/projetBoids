#include "object.hpp"
#include <iostream>
#include <string>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../src-common/tiny_obj_loader.h"

Object::Object() = default;

void Object::setInputfile(std::string filename)
{
    inputfile = "../assets/models/" + filename + ".obj";
};

void Object::set(VBO& vbo)
{
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vbo.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    vbo.debind();
}

void Object::load()
{
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), "../assets/models/");

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

void Object::draw()
{
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
};

std::vector<glimac::ShapeVertex> Object::getVertices()
{
    return vertices;
};