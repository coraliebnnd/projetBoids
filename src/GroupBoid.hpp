#pragma once

#include <vector>
#include "Boid.hpp"

class GroupBoid {
private:
    std::vector<Boid> group;

public:
    // default constructor
    GroupBoid();

    /*METHODS*/

    void                       addBoid(Boid&);
    void                       removeBoid(Boid&);
    void                       moveBoids(float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float average, float alignement, float forceAlignement);
    std::vector<Boid>          getGroup();
    glm::vec<DIMENSION, float> getAverageDirection();
};