#include <cstdlib>
#include <iostream>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "Force.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

const glm::vec<DIMENSION, float> WALL{1.};

int main()
{
    Boid            boid;
    ForceWall       forceWall;
    ForceAttraction forceAttraction;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        boid.updateDirection(forceWall.calcForce(boid, WALL) + forceAttraction.calcForce(boid, ctx.mouse()));
        boid.updatePosition();
        ctx.background(p6::NamedColor::Blue);
        ctx.circle(
            p6::Center{boid.getPosition()},
            p6::Radius{0.06f}
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}