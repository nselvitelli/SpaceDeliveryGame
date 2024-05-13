#pragma once

#include "glm/vec3.hpp"

struct OrbitData {
    glm::vec3 originOffset;
    glm::vec3 axis;
    float angle;
    float angleSpeed;
};