#include "simulation/Planet.hpp"

#include <iostream>

#include "render/Sphere.hpp"


Planet::Planet(glm::vec3 location, float radius, std::vector<Planet*> moons) 
    : m_location(location), 
      m_radius(radius),
      m_moons(moons) {

    m_object = new Sphere();
    m_object->SetTextureFromLibrary("rock");
    m_sceneNode = new SceneNode(m_object);
    m_defaultColor = glm::vec3(0.0f, 1.0f, 0.0f);
    ResetColor();

    for(auto& moon : m_moons) {
        m_sceneNode->AddChild(moon->PlanetNode());
    }    
}

Planet::~Planet() {

}

void Planet::Update(float deltaTime) {
    m_sceneNode->GetLocalTransform().LoadIdentity();
    m_sceneNode->GetLocalTransform().Translate(m_location.x, m_location.y, m_location.z);

    if(m_orbitDataOptional.has_value()) {
        m_sceneNode->GetLocalTransform().Rotate(m_orbitDataOptional->angle,
                                                m_orbitDataOptional->axis.x,
                                                m_orbitDataOptional->axis.y,
                                                m_orbitDataOptional->axis.z);
        m_sceneNode->GetLocalTransform().Translate(m_orbitDataOptional->originOffset.x,
                                                    m_orbitDataOptional->originOffset.y,
                                                    m_orbitDataOptional->originOffset.z);
        m_orbitDataOptional->angle += m_orbitDataOptional->angleSpeed * deltaTime;
    }

    m_sceneNode->GetLocalTransform().Scale(m_radius, m_radius, m_radius);

    for(auto& moon : m_moons) {
        moon->Update(deltaTime);
    }
}

glm::vec3 Planet::WorldLocation() {
    return glm::vec3(m_sceneNode->GetWorldTransform().GetInternalMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Planet::AddOrbit(OrbitData orbitData) {
    m_orbitDataOptional = orbitData;
}