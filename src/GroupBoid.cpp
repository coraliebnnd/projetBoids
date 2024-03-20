#include "GroupBoid.hpp"
#include <iostream>
#include "Boid.hpp"
#include "Force.hpp"
#include "glm/ext/quaternion_geometric.hpp"

GroupBoid::GroupBoid()
{
    for (int i = 0; i < 50; i++)
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

void GroupBoid::moveBoids(float cohesion, float separation, float forceCohesion, float forceSeparation, float mur, float forceMur, float forceAverage, float alignement, float forceAlignement)
{
    // glm::vec<DIMENSION, float> average = getAverageDirection();
    for (Boid& boid : group)
    {
        // ENLEVER CLASS FORCE ET METTRE MES FONCTIONS ICI
        glm::vec<DIMENSION, float> average{0.f};
        int                        nbrAlignement = 0;
        Force                      force;
        for (const Boid& otherBoid : group)
        {
            if (&boid == &otherBoid)
                continue;
            glm::vec<DIMENSION, float> f = force.calcForce(boid, otherBoid, cohesion, separation, forceCohesion, forceSeparation, mur, forceMur, alignement, forceAlignement, average, nbrAlignement);
            std::cout << f.x << " " << f.y << "\n";
            boid.updateDirection(f);
        }
        boid.updateDirection(glm::normalize(average) * forceAverage);
        boid.updatePosition();
    }
}

std::vector<Boid> GroupBoid::getGroup()
{
    return group;
}

glm::vec<DIMENSION, float> GroupBoid::getAverageDirection()
{
    glm::vec<DIMENSION, float> average{0.f};
    for (Boid& Boid : group)
    {
        average += Boid.getDirection();
    }
    return glm::normalize(average);
}