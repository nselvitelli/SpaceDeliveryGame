#include "render/PlaceMarker.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>


PlaceMarker::PlaceMarker(Object* object,
                std::string onScreenTextureName,
                std::string offScreenTextureName) : GUINode(object) {
    m_worldTarget = nullptr;
    m_screenCoords = glm::vec3(-1.0f, -1.0f, 0.0f); // init offscreen, set z to zero so 
    m_reachedRadius = 1.0f;
    m_onScreenTextureName = onScreenTextureName;
    m_offScreenTextureName = offScreenTextureName;
    Resize(50, 50);
}

PlaceMarker::~PlaceMarker() {

}

bool PlaceMarker::PlayerReachedMarker() {
    if(m_worldTarget == nullptr) {
        return true;
    }
    glm::vec3 loc = m_worldTarget->WorldLocation();
    return glm::length(m_playerWorldCoords - loc) < m_worldTarget->Radius() + 0.1f;
}

int PlaceMarker::UpdateTarget(Planet* worldTarget) {
    int score = 0;
    if(m_worldTarget != nullptr) {
        score = std::fmax(1.0f, (5.0f - m_worldTarget->Radius())) * 10;
    }
    m_worldTarget = worldTarget;
    return score;
}

void PlaceMarker::UpdateWorldToScreenCoords(glm::vec3 playerWorldCoords, glm::mat4 projectionMatrix, Camera* camera) {

    // Do not display
    if(m_worldTarget == nullptr) {
        m_screenCoords = glm::vec3(-1.0f, -1.0f, 10.0f);
        return;
    }

    m_playerWorldCoords = playerWorldCoords;

    // how many screen pixels away from target
    // if value is [-1, 1] then in bounds of camera
    glm::vec4 target = glm::vec4(m_worldTarget->WorldLocation(), 1.0f);
    glm::vec4 screenCoords = projectionMatrix * camera->GetWorldToViewmatrix() * target;

    // convert to screen points based on screen dimensions
    m_screenCoords = glm::vec3((m_screenWidth/2.0f) + (screenCoords.x * (m_screenWidth/2.0f) / screenCoords.z),
                               (m_screenHeight/2.0f) + (-screenCoords.y * (m_screenHeight/2.0f) / screenCoords.z),
                               screenCoords.z);
    

    if(m_screenCoords.x > 0 && m_screenCoords.x < m_screenWidth
    && m_screenCoords.y > 0 && m_screenCoords.y < m_screenHeight
    && m_screenCoords.z > 0.0f) {
        m_object->SetTextureFromLibrary(m_onScreenTextureName);
    }
    else {
        m_object->SetTextureFromLibrary(m_offScreenTextureName);
    }


    m_screenCoords.x = std::clamp(m_screenCoords.x, 25.0f, (float)(m_screenWidth) - 25.0f);
    m_screenCoords.y = std::clamp(m_screenCoords.y, 25.0f, (float)(m_screenHeight) - 25.0f);
    
    if(m_screenCoords.z < 0.0f) {
        m_screenCoords.x = (m_screenCoords.x - (m_screenWidth/2.0f) > 0.0f) ? 25.0f : (float)(m_screenWidth) - 25.0f;
        m_screenCoords.y = (m_screenCoords.y - (m_screenHeight/2.0f) > 0.0f) ? 25.0f : (float)(m_screenHeight) - 25.0f; 
    }

    // std::cout << "d: " << glm::length(m_playerWorldCoords - glm::vec3(target))
    //             // << "\t" << target.x << " " << target.y << " " << target.z 
    //             << "\tr: " << m_worldTarget->Radius() << std::endl;
    
    PlaceOnScreen(m_screenCoords.x - 25, m_screenCoords.y - 25, 1);
    Resize(50, 50);
}