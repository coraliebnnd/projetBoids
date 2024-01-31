#include "Boid.hpp"
#include "glm/fwd.hpp"

Boid::Boid()
{
    // need to make coord random
    position  = glm::vec<DIMENSION, float>{0.1f};
    direction = glm::vec<DIMENSION, float>{0.001f};
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
    position = position + direction;
}

void Boid::updateDirection(glm::vec<DIMENSION, float> newDirection)
{
    direction += newDirection;
}