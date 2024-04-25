#include "Boid.hpp"
#include "glm/common.hpp"
#include "random.hpp"

Boid::Boid()
    : size(glm::abs(laplaceRandom(2, 4)) * 0.005)
{
    position  = glm::vec<DIMENSION, float>{uniform(-1., 1), uniform(-1., 1), uniform(-1., 1)};
    direction = glm::vec<DIMENSION, float>{RAND(), RAND(), RAND()};
};

glm::vec<DIMENSION, float> Boid::getPosition() const
{
    {
        return position;
    };
}

glm::vec<DIMENSION, float> Boid::getDirection() const
{
    {
        return direction;
    };
}

Boid::Boid(const glm::vec<DIMENSION, float> newPosition)
    : position{newPosition}
{
}

void Boid::updatePosition()
{
    // std::cout << direction.x << " " << direction.y << "\n";
    position = position + direction;
}

void Boid::updateDirection(glm::vec<DIMENSION, float> force)
{
    direction += force * 0.00001f;

    direction = glm::normalize(direction) * 0.001f;
}

void Boid::setSize(float newSize)
{
    size = newSize;
}

float Boid::getSize() const
{
    return size;
}