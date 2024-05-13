#include "render/Skybox.hpp"

#include "render/Error.hpp"

#include <vector>

Skybox::Skybox(std::string file_right, 
            std::string file_left,
            std::string file_top,
            std::string file_bottom,
            std::string file_front,
            std::string file_back) {
    
    m_right = new Image(file_right);
    m_right->LoadPPM(true);
    m_left = new Image(file_left);
    m_left->LoadPPM(true);
    m_top = new Image(file_top);
    m_top->LoadPPM(true);
    m_bottom = new Image(file_bottom);
    m_bottom->LoadPPM(true);
    m_front = new Image(file_front);
    m_front->LoadPPM(true);
    m_back = new Image(file_back);
    m_back->LoadPPM(true);

    // Setup shaders for the node.
	std::string vertexShader = m_shader.LoadShader("./shaders/skybox_vert.glsl");
	std::string fragmentShader = m_shader.LoadShader("./shaders/skybox_frag.glsl");
	// Actually create our shader
	m_shader.CreateShader(vertexShader,fragmentShader);  

    LoadSkyboxVertexBufferLayout();
    LoadSkyboxTextures();
}

Skybox::~Skybox() {

}

void Skybox::Draw() {
    // Depth test passes when val equal to depth buffer's content
    glDepthFunc(GL_LEQUAL); 
    Bind();
    glDrawElements(GL_TRIANGLES,
                   36, // 36 indices in a cube
                   GL_UNSIGNED_INT,
                        nullptr); 
    glDepthFunc(GL_LESS);
}


void Skybox::Update(glm::mat4 projectionMatrix, Camera* camera) {

    m_shader.Bind();

    m_shader.SetUniform1i("skybox", 0);

    m_shader.SetUniformMatrix4fv("model", &m_transform.GetInternalMatrix()[0][0]);
    m_shader.SetUniformMatrix4fv("view", &camera->GetWorldToViewmatrix()[0][0]);
	m_shader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);
}

void Skybox::UpdateLocation(glm::vec3 location) {
    m_transform.LoadIdentity();
    m_transform.Translate(location.x, location.y, location.z);
}

void Skybox::Bind() {
    m_vertexBufferLayout.Bind();
    m_shader.Bind();

    glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}


void Skybox::LoadSkyboxTextures() {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, m_right->GetWidth(), m_right->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_right->GetPixelDataPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, m_left->GetWidth(), m_left->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_left->GetPixelDataPtr());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, m_top->GetWidth(), m_top->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_top->GetPixelDataPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, m_bottom->GetWidth(), m_bottom->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_bottom->GetPixelDataPtr());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, m_front->GetWidth(), m_front->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_front->GetPixelDataPtr());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, m_back->GetWidth(), m_back->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_back->GetPixelDataPtr());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::LoadSkyboxVertexBufferLayout() {
    std::vector<GLuint> indices {
        2, 6, 7,
        2, 3, 7,
        0, 4, 5,
        0, 1, 5,
        0, 2, 6,
        0, 4, 6,
        1, 3, 7,
        1, 5, 7,
        0, 2, 3,
        0, 1, 3,
        4, 6, 7,
        4, 5, 7
    };

    std::vector<GLfloat> vertices {
        -1, -1,  1,
         1, -1,  1,
        -1,  1,  1,
         1,  1,  1,
        -1, -1, -1,
         1, -1, -1,
        -1,  1, -1,
         1,  1, -1
    };

    m_vertexBufferLayout.CreatePositionBufferLayout(vertices.size(), indices.size(), vertices.data(), indices.data());
}