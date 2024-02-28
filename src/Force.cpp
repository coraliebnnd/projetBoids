#include "Force.hpp"
#include <cmath>
#include <cstdlib>
#include "Boid.hpp"
#include "glm/detail/qualifier.hpp"

Force::Force() = default;

glm::vec<DIMENSION, float> Force::calcForce(Boid boid, Boid otherBoid) const
{
    glm::vec<DIMENSION, float> force{0.f};

    glm::vec<DIMENSION, float> vector = otherBoid.getPosition() - boid.getPosition();

    // glm::length(vec) -> donne la norme du vecteur vec
    float distance = glm::length(vector);

    if (distance < 1.f)
    {
        force += otherBoid.getDirection();
    }

    force = calcForceWall(boid);

    force += boidForce(vector, distance);

    return force;
};

glm::vec<DIMENSION, float> Force::boidForce(glm::vec<DIMENSION, float> vector, float distance) const
{
    glm::vec<DIMENSION, float> force{0.f};

    if (distance < 1. && distance > 0.2)
    {
        force += vector / distance;
    }

    if (distance <= 0.2)
    {
        force += -vector / distance;
    }

    return force * 0.01f;
};

glm::vec<DIMENSION, float> Force::calcForceWall(Boid boid) const
{
    // mettre sous forme de for
    double wallDistanceX = 1 - std::abs(boid.getPosition().x);
    double wallDistanceY = 1 - std::abs(boid.getPosition().y);

    // créer un vec
    double forceX = 0;
    double forceY = 0;

    if ((wallDistanceX < 0.2))
    {
        forceX = -boid.getPosition().x / std::abs(boid.getPosition().x) / wallDistanceX;
    }

    if ((wallDistanceY < 0.2))
    {
        forceY = -boid.getPosition().y / std::abs(boid.getPosition().y) / wallDistanceY;
    }

    return glm::vec<DIMENSION, float>{forceX, forceY} * 0.005f;
};