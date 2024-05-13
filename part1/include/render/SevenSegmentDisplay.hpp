#pragma once

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "render/GUI.hpp"
#include "render/GUINode.hpp"

class SevenSegmentDisplay {
public:
    SevenSegmentDisplay(std::vector<std::string> digitTextureNames,
                        GUI* gui,
                        glm::vec3 screenPosition,
                        glm::vec2 dimensions,
                        int numDigits = 4, 
                        int initialNumber = 0);

    ~SevenSegmentDisplay();

    void Update();
    void SetNumber(int number);
    void Increment(int number = 1);

private:
    int m_numDigits;
    int m_displayNumber;
    int m_targetNumber;

    std::vector<std::string> m_digitTextureNames;
    std::vector<GUINode*> m_digitNodes;
};