#include <imgui.h>
#include <cstdlib>
#include <iostream>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "Force.hpp"
#include "GroupBoid.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

float cohesion        = 0.5f;
float separation      = 0.5f;
float forceCohesion   = 1.f;
float forceSeparation = 1.f;
float mur             = 0.f;
float forceMur        = 1.f;
float forceAverage    = 1.f;
float alignement      = 0.5f;
float forceAlignement = 1.f;

int main()
{
    GroupBoid group;
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("facteurs");
        ImGui::SliderFloat("cohesion", &cohesion, 0.0f, 1.0f);
        ImGui::SliderFloat("separation", &separation, 0.0f, 1.0f);
        ImGui::SliderFloat("force cohesion", &forceCohesion, 0.0f, 100.0f);
        ImGui::SliderFloat("force separation", &forceSeparation, 0.0f, 100.0f);
        ImGui::SliderFloat("distance au mur", &mur, 0.0f, 1.0f);
        ImGui::SliderFloat("force du mur", &forceMur, 0.0f, 100.0f);
        ImGui::SliderFloat("force average", &forceAverage, 0.0f, 100.0f);
        ImGui::SliderFloat("distance au mur", &alignement, 0.0f, 1.0f);
        ImGui::SliderFloat("force du mur", &forceAlignement, 0.0f, 100.0f);
        ImGui::End();

        group.moveBoids(cohesion, separation, forceCohesion, forceSeparation, mur, forceMur, forceAverage, alignement, forceAlignement);
        ctx.background(p6::NamedColor::AshGrey);
        for (Boid& boid : group.getGroup())
        {
            ctx.circle(
                p6::Center{boid.getPosition()},
                p6::Radius{0.04f}
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