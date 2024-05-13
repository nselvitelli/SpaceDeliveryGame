#include "simulation/World.hpp"

#include "misc/RandomGenerator.hpp"

#include <iostream>

World::World() {
    m_worldRoot = new SceneNode(nullptr);
    m_targetPlanet = nullptr;
    this->GeneratePlanets();

    std::cout << "World Generated (" << m_planets.size() << " planets)" << std::endl;
    
    for(auto& planet : m_planets) {
        m_worldRoot->AddChild(planet->PlanetNode());
    }

    m_worldRoot->AddChild(m_player.PlayerNode());
}

World::~World() {

}

void World::Update(float deltaTime) {
    m_player.Update(deltaTime);

    m_worldRoot->GetLocalTransform().LoadIdentity();

    for(auto& planet : m_planets) {
        planet->Update(deltaTime);
    }
}


Planet* World::GetNewTarget() {
    if(m_targetPlanet != nullptr) {
        m_targetPlanet->ResetColor();
    }

    RandomGenerator rand;
    int index = rand.randomInt() % m_planets.size();
    m_targetPlanet = m_planets[index];

    // one extra so still a chance to select the main planet
    int moonIndex = rand.randomInt() % (m_targetPlanet->NumMoons() + 1);
    if(moonIndex > 0) {
        m_targetPlanet = m_targetPlanet->GetMoon(moonIndex - 1);
    }

    m_targetPlanet->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    return m_targetPlanet;
}


void World::GeneratePlanets() {

    int numPlanets = 100;
    float range = 150.0f;
    glm::vec3 lowerBound = glm::vec3(-range, -range, -range);
    glm::vec3 upperBound = glm::vec3(range, range, range);
    float minPlanetRadius = 0.25f;
    float maxPlanetRadius = 5.0f;
    int maxMoons = 2;

    RandomGenerator rand;

    for(int i = 0; i < numPlanets; i++) {

        int numMoons = rand.randomInt() % maxMoons;

        Planet* planet = GenerateSinglePlanet(rand,
                                       lowerBound,
                                       upperBound,
                                       minPlanetRadius,
                                       maxPlanetRadius,
                                       numMoons);

        m_planets.push_back(planet);
    }
}

Planet* World::GenerateSinglePlanet(RandomGenerator& rand,
                             glm::vec3 lowerBound,
                             glm::vec3 upperBound,
                             float minPlanetRadius,
                             float maxPlanetRadius,
                             int numMoons) {
    glm::vec3 location = glm::vec3(rand.randFromRange(lowerBound.x, upperBound.x),
                                    rand.randFromRange(lowerBound.y, upperBound.y),
                                    rand.randFromRange(lowerBound.z, upperBound.z));
    float radius = rand.randFromRange(minPlanetRadius, maxPlanetRadius);

    std::vector<Planet*> moons;
    for(int i = 0; i < numMoons; i++) {
        float minOffset = radius * 1.5f;
        float maxOffset = radius * 3.0f;
        float minAngleSpeed = 0.01f;
        float maxAngleSpeed = 0.5f;
        float minRadius = radius / 15.0f;
        float maxRadius = radius / 5.0f;

        moons.push_back(GenerateMoon(minOffset, maxOffset, minAngleSpeed, maxAngleSpeed, minRadius, maxRadius));
    }

    Planet* planet = new Planet(location, radius, moons);

    return planet;
}

Planet* World::GenerateMoon(float minOffset,
                     float maxOffset,
                     float minAngleSpeed,
                     float maxAngleSpeed,
                     float minRadius,
                     float maxRadius) {
    RandomGenerator rand;
    glm::vec3 offset = glm::vec3(rand.randFromRange(minOffset, maxOffset), 0.0f, 0.0f);
    glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
    float angle = rand.randFromRange(0.0f, 6.28318); // 0 -> ~2pi

    float angleSpeed = rand.randFromRange(minAngleSpeed, maxAngleSpeed);
    if(rand.randomInt() % 2 == 0) {
        angleSpeed *= -1.0f;
    }

    OrbitData data{offset, axis, angle, angleSpeed};

    float radius = rand.randFromRange(minRadius, maxRadius);

    Planet* moon = new Planet(glm::vec3(0.0f), radius, std::vector<Planet*>());
    moon->AddOrbit(data);
    moon->SetDefaultColor(glm::vec3(1.0f, 0.0f, 1.0f));

    return moon;
}