#pragma once

#include "render/GUINode.hpp"

#include <vector>
#include <string>

class GUI {
public:
    GUI(int screenWidth, int screenHeight);
    ~GUI();
    void Update();
    void Draw();
    void AddNode(GUINode* node);
    void LoadQuadAndAddToScreen(std::string textureName, int x, int y, int z, int width, int height);
private:
    std::vector<GUINode*> m_nodes;
    int m_screenWidth;
    int m_screenHeight;
};