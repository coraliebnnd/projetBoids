#include <imgui.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "../src-common/glimac/common.hpp"
#include "../src-common/glimac/default_shader.hpp"
#include "../src-common/glimac/sphere_vertices.hpp"
#include "Boid.hpp"
#include "GroupBoid.hpp"
#include "doctest/doctest.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"

struct BoidProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    BoidProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
    }
};

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

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1, 32, 32);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        sliders();
        group.moveBoids(cohesion, separation, forceCohesion, forceSeparation, mur, forceMur, alignement, forceAlignement);

        ctx.background(p6::NamedColor::AshGrey);
        for (Boid& boid : group.getGroup())
        {
            ctx.circle(
                p6::Center{boid.getPosition()},
                p6::Radius{0.01f}
            );
        }

        ctx.fill = {0., 0., 0., 0.};
        ctx.square(
            p6::Center{0, 0},
            p6::Radius{1}
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}