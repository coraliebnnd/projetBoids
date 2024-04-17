#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#define TINYOBJLOADER_IMPLEMENTATION
#include <VAO.hpp>
#include "../src-common/glimac/TrackballCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "../src-common/tiny_obj_loader.h"
#include "Boid.hpp"
#include "GroupBoid.hpp"
#include "VBO.hpp"
#include "doctest/doctest.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"

float cohesion        = 0.3f;
float separation      = 0.15f;
float forceCohesion   = 10.f;
float forceSeparation = 1.f;
float mur             = 0.2f;
float forceMur        = 1.f;
float alignement      = 0.3f;
float forceAlignement = 50.f;

void sliders()
{
    ImGui::Begin("facteurs");
    ImGui::SliderFloat("cohesion", &cohesion, 0.0f, 1.0f);
    ImGui::SliderFloat("separation", &separation, 0.0f, 1.0f);
    ImGui::SliderFloat("force cohesion", &forceCohesion, 0.0f, 100.0f);
    ImGui::SliderFloat("force separation", &forceSeparation, 0.0f, 100.0f);
    ImGui::SliderFloat("distance au mur", &mur, 0.0f, 1.0f);
    ImGui::SliderFloat("force du mur", &forceMur, 0.0f, 100.0f);
    ImGui::SliderFloat("alignement", &alignement, 0.0f, 1.0f);
    ImGui::SliderFloat("force alignement", &forceAlignement, 0.0f, 100.0f);
    ImGui::End();
}

struct BoidProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    BoidProgram()
        : m_Program{p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/tex3D.fs.glsl")}, uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), uTexture(glGetUniformLocation(m_Program.id(), "uTexture"))
    {
    }
};

int main()
{
    GroupBoid group;
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    /************ CAMERA ***************/
    TrackballCamera trackBallCamera;

    // chargement des shaders
    BoidProgram boidProgram{};

    /************ OBJECTS **************/
    std::string                      inputfile = "../assets/models/jellyfish.obj";
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

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

    std::vector<glimac::ShapeVertex> m_vertices;

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

                m_vertices.push_back(newVertex);
            }
            index_offset += fv;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1, 32, 32);

    // VBO
    VBO vbo;
    vbo.bind();
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glimac::ShapeVertex), m_vertices.data(), GL_STATIC_DRAW);

    vbo.debind();

    // VAO
    VAO vao;
    vao.bind();

    static constexpr GLuint vertex_attr_position  = 0;
    static constexpr GLuint vertex_attr_normal    = 1;
    static constexpr GLuint vertex_attr_texCoords = 2;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_normal);
    glEnableVertexAttribArray(vertex_attr_texCoords);

    vbo.bind();

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    glEnable(GL_DEPTH_TEST);

    vbo.debind();

    vao.debind();

    // last position mouse
    float lastX = 0;
    float lastY = 0;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /************ CAMERA ***************/
        ctx.mouse_dragged = [&](p6::MouseDrag drag) {
            float deltaX = drag.position.x - lastX;
            float deltaY = drag.position.y - lastY;

            if (lastX != 0 && lastY != 0)
            {
                // camera autour d'un objet
                trackBallCamera.rotateLeft(-deltaX * 50.f);
                trackBallCamera.rotateUp(deltaY * 50.f);
            }

            lastX = drag.position.x;
            lastY = drag.position.y;
        };

        if (!ctx.mouse_button_is_pressed(p6::Button::Right))
        {
            lastX = 0;
            lastY = 0;
        }

        // camera autour d'un object
        ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
            trackBallCamera.moveFront(-scroll.dy);
        };

        /*** VIEW MATRIX ***/
        glm::mat4 viewMatrix = trackBallCamera.getViewMatrix();

        // vao
        vao.bind();

        const glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        boidProgram.m_Program.use();

        // texture
        // glUniform1i(boidProgram.uTexture, 0);

        // // texture
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, textureLune);

        sliders();
        group.moveBoids(cohesion, separation, forceCohesion, forceSeparation, mur, forceMur, alignement, forceAlignement);

        ctx.background(p6::NamedColor::SmokyBlack);
        for (Boid& boid : group.getGroup())
        {
            /**********MATRICES BOID**********/

            // matrix model
            glm::mat4 modelMatrixBoid = glm::translate(glm::mat4(1.0f), boid.getPosition());
            modelMatrixBoid           = glm::scale(modelMatrixBoid, glm::vec3(0.01, 0.01, 0.01));

            // matrix model-vue
            glm::mat4 modelViewMatrixBoid = viewMatrix * modelMatrixBoid;

            // matrix MVP
            glm::mat4 MVPMatrixBoid = projMatrix * modelViewMatrixBoid;

            glUniformMatrix4fv(boidProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrixBoid));
            glUniformMatrix4fv(boidProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrixBoid))));
            glUniformMatrix4fv(boidProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrixBoid));

            // draw boid
            glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        }

        vao.debind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}