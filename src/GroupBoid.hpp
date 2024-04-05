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
    void                       moveBoids(float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float alignement, float forceAlignement);
    std::vector<Boid>          getGroup();
    glm::vec<DIMENSION, float> wallForce(Boid boid, float posWall, int coord, float rayonMur, float forceMur);
    glm::vec<DIMENSION, float> separationForce(glm::vec<DIMENSION, float> vector, float distance, float rayonSeparation, float forceSeparation);
    glm::vec<DIMENSION, float> cohesionForce(glm::vec<DIMENSION, float> vector, float distance, float rayonCohesion, float forceCohesion);
};