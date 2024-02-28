#pragma once

#include "Boid.hpp"

class Force {
public:
    Force();
    glm::vec<DIMENSION, float> calcForce(Boid boid, Boid otherBoid) const;
    glm::vec<DIMENSION, float> boidForce(glm::vec<DIMENSION, float> vector, float distance) const;
    glm::vec<DIMENSION, float> calcForceWall(Boid boid) const;
    ~Force() = default;
};