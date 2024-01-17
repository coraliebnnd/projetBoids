#pragma once

template<typename T>
class Boid {
private:
    T coordX;
    T coordY;
    T coordZ;

public:
    // default constructor
    Boid()
    {
        coordX = 2;
    };

    // constructor.s
    Boid(const T& x, const T& y, const T& z);

    // copy constructor
    Boid(const Boid& b);

    // destructor
    ~Boid();

    /*METHODS*/

    // setter
    void setCoord(const T& x, const T& y, const T& z);

    // getters
    T getCoordX() const;
    T getCoordY() const;
    T getCoordZ() const;
};