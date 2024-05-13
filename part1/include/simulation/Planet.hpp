#pragma once

#include <vector>
#include <optional>

#include "glm/vec3.hpp"

#include "render/SceneNode.hpp"
#include "render/Object.hpp"

#include "simulation/OrbitData.hpp"

class Planet {
public:
    Planet(glm::vec3 location, float radius, std::vector<Planet*> moons);
    ~Planet();
    void Update(float deltaTime);

    glm::vec3 WorldLocation();

    void AddOrbit(OrbitData orbitData);

    inline SceneNode* PlanetNode() { return m_sceneNode; }
    inline float Radius() { return m_radius; }

    inline void SetColor(glm::vec3 color) {
        if(m_sceneNode != nullptr) {
            m_sceneNode->SetColor(color);
        }
    }

    inline void SetDefaultColor(glm::vec3 color) { 
        m_defaultColor = color;
        ResetColor();
    }
    inline void ResetColor() { SetColor(m_defaultColor); }

    inline int NumMoons() { return m_moons.size(); }
    inline Planet* GetMoon(int index) { return m_moons[index]; }

private:
    glm::vec3 m_location;
    float m_radius;
    std::vector<Planet*> m_moons;

    std::optional<OrbitData> m_orbitDataOptional;

    glm::vec3 m_defaultColor;

    Object* m_object;
    SceneNode* m_sceneNode;
};