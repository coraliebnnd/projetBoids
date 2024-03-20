#include "Force.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Boid.hpp"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/quaternion_geometric.hpp"

const int coordX = 0;
const int coordY = 1;

Force::Force() = default;

glm::vec<DIMENSION, float> Force::calcForce(Boid boid, Boid otherBoid, float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float alignement, float forceAlignement, glm::vec<DIMENSION, float> average, int nbrAlignement) const
{
    glm::vec<DIMENSION, float> force{0.f};

    glm::vec<DIMENSION, float> vector = otherBoid.getPosition() - boid.getPosition();

    // glm::length(vec) -> donne la norme du vecteur vec
    float distance = glm::length(vector);

    if (boid.getPosition().x < -0)
    {
        force += calcForceWall(boid, -1, coordX, mur, forceMur);
    }
    else
    {
        force += calcForceWall(boid, 1, coordX, mur, forceMur);
    }

    if (boid.getPosition().y < 0)
    {
        force += calcForceWall(boid, -1, coordY, mur, forceMur);
    }
    else
    {
        force += calcForceWall(boid, 1, coordY, mur, forceMur);
    }

    if (distance <= cohesion)
    {
        force += cohesionForce(vector, distance, cohesion, forceCohesion);
    }

    if (distance <= separation)
    {
        force += separationForce(vector, distance, separation, forceSeparation);
    }

    if (distance <= alignement)
    {
        nbrAlignement = 0;
        average += otherBoid.getDirection();
    }

    return force;
};

glm::vec<DIMENSION, float> Force::cohesionForce(glm::vec<DIMENSION, float> vector, float distance, float cohesion, float forceCohesion) const
{
    glm::vec<DIMENSION, float> force{0.f};

    if (cohesion - distance >= 0)
    {
        force += (cohesion - distance) * vector;
    }

    return force * forceCohesion;
};

glm::vec<DIMENSION, float> Force::separationForce(glm::vec<DIMENSION, float> vector, float distance, float separation, float forceSeparation) const
{
    glm::vec<DIMENSION, float> force{0.f};

    if (separation - distance >= 0)
    {
        force += -(separation - distance) * vector / glm::length(vector) / glm::length(vector);
    }

    return force * forceSeparation;
};

// glm::vec<DIMENSION, float> Force::alignementForce(glm::vec<DIMENSION, float> vector, float distance, float alignement, float forceAlignement) const
// {
//     glm::vec<DIMENSION, float> force{0.f};

//     if (alignement - distance >= 0)
//     {
//         force += -(alignement - distance) * vector / glm::length(vector);
//     }

//     return force * forceAlignement;
// };

glm::vec<DIMENSION, float> Force::calcForceWall(Boid boid, float posWall, int coord, float mur, float forceMur) const
{
    double wallDistance = (posWall - boid.getPosition()[coord]) * posWall;
    double f            = 0;

    if ((wallDistance < mur))
    {
        const float x = (mur - wallDistance); // Toujours > 0 car il y a un if au dessus
        f             = x * x;
    }

    glm::vec<DIMENSION, float> force{0.f};
    force[coord] = -posWall * f;
    return force * forceMur;
};