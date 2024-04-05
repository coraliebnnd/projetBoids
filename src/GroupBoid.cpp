#include "GroupBoid.hpp"
#include <iostream>
#include "Boid.hpp"
#include "glm/ext/quaternion_geometric.hpp"

const int coordX = 0;
const int coordY = 1;

GroupBoid::GroupBoid()
{
    for (int i = 0; i < 100; i++)
    {
        Boid newBoid;
        addBoid(newBoid);
    }
}

void GroupBoid::addBoid(Boid& boid)
{
    group.push_back(boid);
}

void GroupBoid::removeBoid(Boid& boid)
{
}

void GroupBoid::moveBoids(float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float alignement, float forceAlignement)
{
    ;
    for (Boid& boid : group)
    {
        glm::vec<DIMENSION, float> average{0.f};
        glm::vec<DIMENSION, float> force{0.f};
        for (const Boid& otherBoid : group)
        {
            if (&boid == &otherBoid)
                continue;

            glm::vec<DIMENSION, float> vectorBoidToOtherBoid = otherBoid.getPosition() - boid.getPosition();

            // glm::length(vec) -> donne la norme du vecteur vec
            float distance = glm::length(vectorBoidToOtherBoid);

            if (boid.getPosition().x < -0)
            {
                force += wallForce(boid, -1, coordX, mur, forceMur);
            }
            else
            {
                force += wallForce(boid, 1, coordX, mur, forceMur);
            }

            if (boid.getPosition().y < 0)
            {
                force += wallForce(boid, -1, coordY, mur, forceMur);
            }
            else
            {
                force += wallForce(boid, 1, coordY, mur, forceMur);
            }

            if (distance <= cohesion)
            {
                force += cohesionForce(vectorBoidToOtherBoid, distance, cohesion, forceCohesion);
            }

            if (distance <= separation)
            {
                force += separationForce(vectorBoidToOtherBoid, distance, separation, forceSeparation);
            }

            if (distance <= alignement)
            {
                average += otherBoid.getDirection();
                glm::normalize(average);
            }

            boid.updateDirection(force);
        }
        boid.updateDirection(average * forceAlignement);
        boid.updatePosition();
    }
}

std::vector<Boid> GroupBoid::getGroup()
{
    return group;
}

glm::vec<DIMENSION, float> GroupBoid::cohesionForce(glm::vec<DIMENSION, float> vector, float distance, float rayonCohesion, float forceCohesion)
{
    glm::vec<DIMENSION, float> force{0.f};

    if (rayonCohesion - distance >= 0)
    {
        force += (rayonCohesion - distance) * vector;
    }

    return force * forceCohesion;
};

glm::vec<DIMENSION, float> GroupBoid::separationForce(glm::vec<DIMENSION, float> vector, float distance, float rayonSeparation, float forceSeparation)
{
    glm::vec<DIMENSION, float> force{0.f};

    if (rayonSeparation - distance >= 0)
    {
        force += -(rayonSeparation - distance) * vector / glm::length(vector) / glm::length(vector);
    }

    return force * forceSeparation;
};

glm::vec<DIMENSION, float> GroupBoid::wallForce(Boid boid, float posWall, int coord, float rayonMur, float forceMur)
{
    double wallDistance = (posWall - boid.getPosition()[coord]) * posWall;
    double f            = 0;

    if ((wallDistance < rayonMur))
    {
        const float x = (rayonMur - wallDistance); // Toujours > 0 car il y a un if au dessus
        f             = x * x;
    }

    glm::vec<DIMENSION, float> force{0.f};
    force[coord] = -posWall * f;
    return force * forceMur;
};