#include "render/GUI.hpp"


GUI::GUI(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

GUI::~GUI() {

}

void GUI::Update() {
    for(auto& node : m_nodes) {
        node->Update();
    }
}

void GUI::Draw() {
    for(auto& node : m_nodes) {
        node->Draw();
    }
}

void GUI::AddNode(GUINode* node) { 
    node->SetScreenDimensions(m_screenWidth, m_screenHeight);
    m_nodes.push_back(node);
}

// Easy way to add static textured quads to the GUI
void GUI::LoadQuadAndAddToScreen(std::string textureName, int x, int y, int z, int width, int height) {
    Object* test = new Object();
    test->MakeTexturedQuad(textureName);

    GUINode* guiNode = new GUINode(test);
    AddNode(guiNode); // add to GUI first to update width and height

    guiNode->PlaceOnScreen(x, y, z);
    guiNode->Resize(width, height);
}