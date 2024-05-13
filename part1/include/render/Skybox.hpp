#pragma once

#include "render/Image.hpp"
#include "render/Shader.hpp"
#include "render/Camera.hpp"
#include "render/VertexBufferLayout.hpp"
#include "render/Transform.hpp"

#include <string>
#include <glad/glad.h>

#include "glm/vec3.hpp"


class Skybox {
public:
    Skybox(std::string file_right, 
            std::string file_left,
            std::string file_top,
            std::string file_bottom,
            std::string file_front,
            std::string file_back);
    ~Skybox();

    void Draw();
    void Update(glm::mat4 projectionMatrix, Camera* camera);
    void UpdateLocation(glm::vec3 location);

private:

    void Bind();

    void LoadSkyboxTextures();

    void LoadSkyboxVertexBufferLayout();

    Image* m_right;
    Image* m_left;
    Image* m_top;
    Image* m_bottom;
    Image* m_front;
    Image* m_back;

    GLuint m_textureID;
    Shader m_shader;
    Transform m_transform;
    VertexBufferLayout m_vertexBufferLayout;
};