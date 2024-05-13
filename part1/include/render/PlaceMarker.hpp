#pragma once

#include "render/GUINode.hpp"
#include "render/Camera.hpp"

#include "simulation/Planet.hpp"

#include "glm/glm.hpp"

#include <string>

class PlaceMarker : public GUINode {
public:
    PlaceMarker(Object* object, 
                std::string onScreenTextureName,
                std::string offScreenTextureName);
    ~PlaceMarker();

    void UpdateWorldToScreenCoords(glm::vec3 playerWorldCoords, glm::mat4 projectionMatrix, Camera* camera);

    bool PlayerReachedMarker();

    inline void SetReachedRadius(float radius) {
        m_reachedRadius = radius;
    }

    // returns score from last target
    int UpdateTarget(Planet* worldTarget);

private:

    Planet* m_worldTarget;
    glm::vec3 m_playerWorldCoords;

    glm::vec3 m_screenCoords;

    // Names used in the texture library
    std::string m_onScreenTextureName;
    std::string m_offScreenTextureName;

    float m_reachedRadius;
};