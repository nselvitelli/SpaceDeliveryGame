#pragma once

#include <vector>


#include "simulation/Planet.hpp"
#include "simulation/Player.hpp"

#include "render/SceneNode.hpp"

#include "misc/RandomGenerator.hpp"

class World {
public:
    World();
    ~World();
    void Update(float deltaTime);

    Planet* GetNewTarget();

    inline Player& GetPlayer() { return m_player; }
    inline SceneNode* WorldRoot() { return m_worldRoot; }
private:
    void GeneratePlanets();
    Planet* GenerateSinglePlanet(RandomGenerator& rand,
                             glm::vec3 lowerBound,
                             glm::vec3 upperBound,
                             float minPlanetRadius,
                             float maxPlanetRadius,
                             int numMoons);
    Planet* GenerateMoon(float minOffset,
                     float maxOffset,
                     float minAngleSpeed,
                     float maxAngleSpeed,
                     float minRadius,
                     float maxRadius);


    Player m_player;
    std::vector<Planet*> m_planets;
    Planet* m_targetPlanet;

    SceneNode* m_worldRoot;
};