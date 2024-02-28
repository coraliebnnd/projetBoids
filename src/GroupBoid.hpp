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

    void              addBoid(Boid&);
    void              removeBoid(Boid&);
    void              moveBoids();
    std::vector<Boid> getGroup();
};