#pragma once

#include "Boid.hpp"

class Force {
public:
    Force();
    glm::vec<DIMENSION, float> calcForce(Boid boid, Boid otherBoid, float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float alignement, float forceAlignement, glm::vec<DIMENSION, float> average, int nbrAlignement) const;
    glm::vec<DIMENSION, float> separationForce(glm::vec<DIMENSION, float> vector, float distance, float separation, float forceSeparation) const;
    glm::vec<DIMENSION, float> cohesionForce(glm::vec<DIMENSION, float> vector, float distance, float cohesion, float forceCohesion) const;
    glm::vec<DIMENSION, float> alignementForce(glm::vec<DIMENSION, float> vector, float distance, float alignement, float forceAlignement) const;
    glm::vec<DIMENSION, float> calcForceWall(Boid boid, float posWall, int coord, float mur, float forceMur) const;
    ~Force() = default;
};