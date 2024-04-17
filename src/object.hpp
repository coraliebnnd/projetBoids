#pragma once

#include <p6/p6.h>
#include <string>
#include "../src-common/glimac/common.hpp"
#include "../src-common/tiny_obj_loader.h"
#include "Boid.hpp"
#include "glm/fwd.hpp"

class Object {
private:
    p6::Shader m_Program = p6::load_shader("../src/shaders/3D.vs.glsl");

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    std::string                      inputfile;
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<glimac::ShapeVertex> vertices;

public:
    Object();

    void                             setInputfile(std::string filename);
    void                             load();
    std::vector<glimac::ShapeVertex> getVertices();
    GLint                            getUTexture();
    void                             use();
    void                             matrices(glm::mat4 projMatrix, glm::mat4 viewMatrix, glm::vec<DIMENSION, float> position);
    void                             draw();
};