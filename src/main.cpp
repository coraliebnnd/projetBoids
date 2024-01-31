#include <cstdlib>
#include "glm/fwd.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include "Boid.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

int main()
{
    Boid boid;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        boid.updatePosition();
        ctx.background(p6::NamedColor::Blue);
        ctx.circle(
            p6::Center{boid.getPosition()},
            p6::Radius{0.1f}
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}