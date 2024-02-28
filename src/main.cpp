#include <cstdlib>
#include <iostream>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "Force.hpp"
#include "GroupBoid.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

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
        group.moveBoids();
        ctx.background(p6::NamedColor::Blue);
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