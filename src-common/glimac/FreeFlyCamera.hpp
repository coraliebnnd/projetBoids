#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

class FreeFlyCamera {
private:
    glm::vec3 m_position;
    float     m_phi{};
    float     m_theta{};
    glm::vec3 m_FrontVector{};
    glm::vec3 m_LeftVector{};
    glm::vec3 m_UpVector{};

public:
    void computeDirectionVectors()
    {
        m_FrontVector = {std::cos(m_theta) * std::sin(m_phi), std::sin(m_theta), std::cos(m_theta) * std::cos(m_phi)};
        m_LeftVector  = {std::sin(m_phi + p6::PI / 2), 0, std::cos(m_phi + p6::PI / 2)};
        m_UpVector    = {glm::cross(m_FrontVector, m_LeftVector)};
    }

    FreeFlyCamera()
        : m_position(0.f, 0.f, 0.f), m_phi(p6::PI)
    {
        computeDirectionVectors();
    }

    void moveLeft(float t)
    {
        m_position += t * m_LeftVector;
    }

    void moveFront(float t)
    {
        m_position += t * m_FrontVector;
    }

    void rotateLeft(float degrees)
    {
        m_phi += glm::radians(degrees);
        computeDirectionVectors();
    }

    void rotateUp(float degrees)
    {
        m_theta += glm::radians(degrees);
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_FrontVector, m_UpVector);
    }
};