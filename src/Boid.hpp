#pragma once

#include <vector>
#include "glm/detail/qualifier.hpp"
#include "glm/glm.hpp"
#include "random.hpp"

const int DIMENSION = 3;

class Boid {
private:
    glm::vec<DIMENSION, float> position{1.};
    glm::vec<DIMENSION, float> direction{1.};
    float                      size = 0.01;

public:
    // default constructor
    Boid();

    // constructor.s
    explicit Boid(glm::vec<DIMENSION, float> newPosition);

    //     /*METHODS*/

    //     // setter
    void setPosition(glm::vec<DIMENSION, float>& newPosition)
    {
        position = newPosition;
    };
    void setSize(float newSize);

    // getters
    glm::vec<DIMENSION, float> getPosition() const;
    glm::vec<DIMENSION, float> getDirection() const;
    float                      getSize() const;

    void updatePosition();

    void updateDirection(glm::vec<DIMENSION, float> force);
};