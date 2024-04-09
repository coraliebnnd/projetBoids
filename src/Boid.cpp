#include "Boid.hpp"
#include <iostream>
#include <random>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"

double rand01()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen) * 2 - 1;
}

Boid::Boid()
{
    // need to make coord random
    position  = glm::vec<DIMENSION, float>{rand01(), rand01(), rand01()};
    direction = glm::vec<DIMENSION, float>{rand01(), rand01(), rand01()};
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