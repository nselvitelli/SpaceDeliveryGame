#pragma once

#include "render/Shader.hpp"
#include "render/Object.hpp"
#include "render/Transform.hpp"


class GUINode {
public:
    GUINode(Object* object);
    ~GUINode();

    void PlaceOnScreen(int x, int y, int z);
    void Resize(int width, int height);

    void Update();
    void Draw();

    inline Transform& GetTransform() {
        return m_transform;
    }

    inline void ShowOnScreen(bool show=true) {
        m_showOnScreen = show;
    }

    void SetTextureFromLibrary(std::string textureName);

    void SetScreenDimensions(int screenWidth, int screenHeight);

protected:
    Shader m_shader;
    Object* m_object;
    Transform m_transform;

    int m_screenWidth;
    int m_screenHeight;

    bool m_showOnScreen;
};