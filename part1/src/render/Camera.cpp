#include "render/Camera.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"


#include <iostream>


void Camera::MouseLook(int mouseX, int mouseY){

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
    float currentPitch = glm::asin(glm::dot(m_viewDirection, m_upVector));
    float clampedPitch = glm::clamp(currentPitch + pitch, -maxPitch, maxPitch);
    pitch = clampedPitch - currentPitch;

    // Rotate about the upVector
    // m_viewDirection = glm::rotate(m_viewDirection, yaw, m_upVector);
    glm::quat yawQuat = glm::angleAxis(yaw, m_upVector);

    glm::vec3 rightVector = glm::normalize(glm::cross(m_viewDirection, m_upVector));

    // m_viewDirection = glm::rotate(m_viewDirection, pitch, m_rightVector);
    glm::quat pitchQuat = glm::angleAxis(pitch, rightVector);

    glm::quat orientationChange = yawQuat * pitchQuat;
    m_viewDirection = glm::rotate(orientationChange, m_viewDirection);

    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;
}



// OPTIONAL TODO: 
//               The camera could really be improved by
//               updating the eye position along the m_viewDirection.
//               Think about how you can do this for a better camera!

void Camera::MoveForward(float speed){
    m_eyePosition.z -= speed;
}

void Camera::MoveBackward(float speed){
    m_eyePosition.z += speed;
}

void Camera::MoveLeft(float speed){
    m_eyePosition.x -= speed;
}

void Camera::MoveRight(float speed){
    m_eyePosition.x += speed;
}

void Camera::MoveUp(float speed){
    m_eyePosition.y += speed;
}

void Camera::MoveDown(float speed){
    m_eyePosition.y -= speed;
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

void Camera::SetCameraEyePosition(glm::vec3 eyePosition){
    m_eyePosition = eyePosition;
}

void Camera::SetViewDirection(glm::vec3 viewDirection){
    m_viewDirection = viewDirection;
}


float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


Camera::Camera(){
    std::cout << "(Constructor) Created a Camera!\n";
	// Position us at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( m_eyePosition,
                        m_eyePosition + m_viewDirection,
                        m_upVector);
}
