#pragma once

#include <p6/p6.h>
#include <string>
#include "../src-common/glimac/common.hpp"
#include "../src-common/tiny_obj_loader.h"
#include "Boid.hpp"
#include "VBO.hpp"
#include "glm/fwd.hpp"

class Object {
private:
    static constexpr GLuint vertex_attr_position  = 0;
    static constexpr GLuint vertex_attr_normal    = 1;
    static constexpr GLuint vertex_attr_texCoords = 2;

    std::string                      inputfile;
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<glimac::ShapeVertex> vertices;

public:
    Object();
    void                             setInputfile(std::string filename);
    void                             load();
    void                             set(VBO& vbo);
    std::vector<glimac::ShapeVertex> getVertices();
    void                             draw();
};