#include "Force.hpp"
#include <cmath>
#include <cstdlib>
#include "Boid.hpp"
#include "glm/detail/qualifier.hpp"

ForceWall::ForceWall() = default;

// Need to get unit vectors
// unit vector v = v / |v|

// very similar code : can I do only one function ?

glm::vec<DIMENSION, float> ForceWall::calcForce(Boid boid, const glm::vec<DIMENSION, float> obstacle) const
{
    double wallDistanceX = std::abs(obstacle.x) - std::abs(boid.getPosition().x);
    double wallDistanceY = std::abs(obstacle.y) - std::abs(boid.getPosition().y);

    double forceX = 0;
    double forceY = 0;

    if ((wallDistanceX < 0.2))
    {
        forceX = -1. / boid.getPosition().x;
    }

    if ((wallDistanceY < 0.2))
    {
        forceY = -1. / boid.getPosition().y;
    }

    return glm::vec<DIMENSION, float>{forceX, forceY};
};

ForceAttraction::ForceAttraction() = default;

glm::vec<DIMENSION, float> ForceAttraction::calcForce(Boid boid, glm::vec<DIMENSION, float> obstacle) const
{
    double DistanceX = std::abs(std::abs(obstacle.x) - std::abs(boid.getPosition().x));
    double DistanceY = std::abs(std::abs(obstacle.y) - std::abs(boid.getPosition().y));

    double forceX = 0;
    double forceY = 0;

    float distance = sqrt(DistanceX * DistanceX + DistanceY * DistanceY);

    if (DistanceX > 0.05 && DistanceX < 0.4)
    {
        forceX = DistanceX * (obstacle.x / std::abs(obstacle.x));
    }

    if (DistanceY > 0.05 && DistanceY < 0.4)
    {
        forceY = DistanceY * (obstacle.y / std::abs(obstacle.y));
    }

    return glm::vec<DIMENSION, float>{forceX, forceY} / distance * 0.1f;
};