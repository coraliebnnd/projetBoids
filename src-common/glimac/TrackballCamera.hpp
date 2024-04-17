#pragma once

#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

class TrackballCamera {
private:
    float m_Distance{5};
    float m_AngleX{0};
    float m_AngleY{0};

public:
    TrackballCamera() = default;

    void moveFront(float delta)
    {
        m_Distance += delta;
    }

    void rotateLeft(float degrees)
    {
        m_AngleY += degrees;
    }

    void rotateUp(float degrees)
    {
        m_AngleX += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 viewMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -m_Distance});
        viewMatrix           = glm::rotate(viewMatrix, glm::radians(m_AngleY), glm::vec3{0., 1., 0.});
        viewMatrix           = glm::rotate(viewMatrix, glm::radians(m_AngleX), glm::vec3{1., 0., 0.});
        return viewMatrix;
    }
};
