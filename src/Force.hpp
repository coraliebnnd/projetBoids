#pragma once

#include "Boid.hpp"

class Force {
public:
    virtual glm::vec<DIMENSION, float> calcForce(Boid boid, glm::vec<DIMENSION, float> obstacle) const = 0;
    virtual ~Force()                                                                                   = default;
};

class ForceWall : public Force {
public:
    ForceWall();
    glm::vec<DIMENSION, float> calcForce(Boid boid, glm::vec<DIMENSION, float> obstacle) const override;
};

class ForceAttraction : public Force {
public:
    ForceAttraction();
    glm::vec<DIMENSION, float> calcForce(Boid boid, glm::vec<DIMENSION, float> obstacle) const override;
};