#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include "glm/fwd.hpp"
#include "texture.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <VAO.hpp>
#include "../src-common/glimac/TrackballCamera.hpp"
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "Boid.hpp"
#include "GroupBoid.hpp"
#include "VBO.hpp"
#include "doctest/doctest.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "object.hpp"

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

    /************ OBJECTS **************/
    // chargement des shaders
    Object objBoid;
    objBoid.setInputfile("jellyfish");
    objBoid.load();

    /************ TEXTURE **************/
    Texture textureBoid;
    textureBoid.bind();
    textureBoid.set("jellyfish");
    textureBoid.debind();

    // VBO
    VBO vbo;
    vbo.bind();

    vbo.sendData(objBoid.getVertices());
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

        objBoid.use();

        textureBoid.use(objBoid.getUTexture());
        textureBoid.bind();

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

            objBoid.matrices(projMatrix, viewMatrix, boid.getPosition());

            // draw boid
            objBoid.draw();
        }

        vao.debind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}