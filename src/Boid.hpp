#pragma once

#include <vector>
#include "glm/detail/qualifier.hpp"
#include "glm/glm.hpp"

const int DIMENSION = 2;

class Boid {
private:
    glm::vec<DIMENSION, float> position;
    glm::vec<DIMENSION, float> direction;

public:
    // default constructor
    Boid();

    // constructor.s
    Boid(glm::vec<DIMENSION, float> newPosition);

    //     /*METHODS*/

    //     // setter
    void setPosition(glm::vec<DIMENSION, float>& newPosition)
    {
        position = newPosition;
    };

    // getters
    glm::vec<DIMENSION, float> getPosition() const;
    glm::vec<DIMENSION, float> getDirection() const;

    void updatePosition();

    void updateDirection(glm::vec<DIMENSION, float> force);
};