#include "Boid.hpp"
#include "Force.hpp"
#include "glm/fwd.hpp"

Boid::Boid()
{
    // need to make coord random
    position  = glm::vec<DIMENSION, float>{0.1f};
    direction = glm::vec<DIMENSION, float>{1., 0.6};
};

glm::vec<DIMENSION, float> Boid::getPosition() const
{
    {
        return position;
    };
}

Boid::Boid(const glm::vec<DIMENSION, float> newPosition)
{
    position = newPosition;
}

void Boid::updatePosition()
{
    position = position + (direction * 0.002f);
}

void Boid::updateDirection(glm::vec<DIMENSION, float> force)
{
    direction += force;
}