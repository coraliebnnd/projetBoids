#include "GroupBoid.hpp"
#include "Boid.hpp"
#include "Force.hpp"

GroupBoid::GroupBoid()
{
    for (int i = 0; i < 10; i++)
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

void GroupBoid::moveBoids()
{
    for (Boid& boid : group)
    {
        Force force;
        for (Boid& otherBoid : group)
        {
            boid.updateDirection(force.calcForce(boid, otherBoid));
        }
        boid.updatePosition();
    }
}

std::vector<Boid> GroupBoid::getGroup()
{
    return group;
}