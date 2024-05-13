#include "simulation/Player.hpp"

#include <cmath>
#include <iostream>

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"

#include "render/OBJObject.hpp"

Player::Player() {
    m_thrustDirection = glm::vec3(0.0f,0.0f, -1.0f);
    m_numHorizRots = 0;
    m_numVertRots = 0;
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    m_location = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);

    m_thrust_speed = 0;

    // m_playerObject = new OBJObject("/Users/nickselvitelli/Documents/NU/Year 4/Graphics/monorepo-nselvitelli/common/objects/textured_cube/cube.obj");
    m_playerObject = new OBJObject("../common/objects/spaceship.obj");
    m_playerNode = new SceneNode(m_playerObject);
    m_playerNode->SetLockObjectView(true);
    m_playerNode->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

    // obj model transform
    
}

Player::~Player() {

}

void Player::Update(float deltaTime) {

    // movement
    m_velocity = (m_thrust_speed * glm::normalize(m_thrustDirection)) * (deltaTime / m_mass);
    m_location += m_velocity * deltaTime;

    // std::cout << "Pos: " << m_location.x << " " << m_location.y << " " << m_location.z
    //           << "\nVel: " << m_velocity.x << " " << m_velocity.y << " " << m_velocity.z
    //           << "\nDir: " << m_thrustDirection.x << " " << m_thrustDirection.y << " " << m_thrustDirection.z
    //           << "\nThrust: " << m_thrust_speed
    //           << std::endl << std::endl;

    m_playerNode->GetLocalTransform().LoadIdentity();

    m_modelOffset = glm::vec3(0.0f, -0.125f, -0.5f);
    m_playerNode->GetLocalTransform().Translate(m_modelOffset.x, m_modelOffset.y, m_modelOffset.z);
    m_playerNode->GetLocalTransform().Rotate(glm::radians(-90.0f), 1.0f, 0.0f, 0.0f);
    m_playerNode->GetLocalTransform().Scale(0.125f, 0.125f, 0.125f);

    m_playerNode->GetLocalTransform().Rotate(m_turnAngle, 0.0f, 1.0f, 0.0f);

    m_turnAngle *= 0.95f;
}

void Player::MouseLook(int mouseX, int mouseY){

    glm::vec2 newMousePosition(mouseX, mouseY);
    // Little hack for our 'mouse look function'
    // We need this so that we can move our camera
    // for the first time.
    static bool firstLook=true;
    if(true == firstLook){
        firstLook=false;
        m_oldMousePosition = newMousePosition;
    }

    // Detect how much the mouse has moved since the last time
    glm::vec2 mouseDiff = newMousePosition - m_oldMousePosition;
    
    float yaw = glm::radians(-1.0f * mouseDiff.x);
    float pitch = glm::radians(-1.0f * mouseDiff.y);

    float maxPitch = glm::radians(89.0f);
    float currentPitch = glm::asin(glm::dot(m_thrustDirection, m_upVector));
    float clampedPitch = glm::clamp(currentPitch + pitch, -maxPitch, maxPitch);
    pitch = clampedPitch - currentPitch;

    // Rotate about the upVector
    // m_viewDirection = glm::rotate(m_viewDirection, yaw, m_upVector);
    glm::quat yawQuat = glm::angleAxis(yaw, m_upVector);

    glm::vec3 rightVector = glm::normalize(glm::cross(m_thrustDirection, m_upVector));

    // m_viewDirection = glm::rotate(m_viewDirection, pitch, m_rightVector);
    glm::quat pitchQuat = glm::angleAxis(pitch, rightVector);

    glm::quat orientationChange = yawQuat * pitchQuat;
    m_thrustDirection = glm::rotate(orientationChange, m_thrustDirection);

    // rotate player a bit
    m_turnAngle += glm::radians(mouseDiff.x * m_turnAngleSpeed);
    m_turnAngle = std::clamp(m_turnAngle, -m_maxTurnAngle, m_maxTurnAngle);

    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;
}


void Player::SpeedUp(float deltaTime) {
    m_thrust_speed = fmin(m_thrust_speed + 10, m_max_thrust);
}

void Player::SlowDown(float deltaTime) {
    m_thrust_speed = fmax(m_thrust_speed - 10, 0);
}

void Player::Overdrive(bool enable) {
    if(enable) {
        m_thrust_speed = m_overdrive;
    }
    else {
        m_thrust_speed = fmin(m_thrust_speed, m_max_thrust);
    }
}