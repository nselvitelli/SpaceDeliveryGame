#pragma once

#include "glm/glm.hpp"

#include "render/Object.hpp"
#include "render/SceneNode.hpp"

class Player {
public:
    Player();
    ~Player();
    void Update(float deltaTime);

    void MouseLook(int mouseX, int mouseY);

    void SpeedUp(float deltaTime);
    void SlowDown(float deltaTime);
    void Overdrive(bool enable);

    inline glm::vec3 Location() { return m_location; };
    inline glm::vec3 ThrustDirection() { return m_thrustDirection; };
    inline SceneNode* PlayerNode() { return m_playerNode; };

    inline void OverridePreviousMousePosition(glm::vec2 position) {
        m_oldMousePosition = position;
    }
private:
    glm::vec3 m_location;
    glm::vec3 m_velocity;
    
    glm::vec3 m_thrustDirection;
    glm::vec3 m_upVector;

    float m_thrust_speed;
    const float m_mass{1.0f};
    const float m_max_thrust{500.0f};
    const float m_overdrive{2000.0f};

    float m_turnAngle;
    const float m_turnAngleSpeed{1.5f};
    const float m_maxTurnAngle{1.5f};

    glm::vec2 m_oldMousePosition;
    int m_numVertRots;
    int m_numHorizRots;

    Object* m_playerObject;
    SceneNode* m_playerNode;
    glm::vec3 m_modelOffset;
};