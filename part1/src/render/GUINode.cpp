#include "render/GUINode.hpp"


#include "glm/glm.hpp"

GUINode::GUINode(Object* object) {
    m_object = object;

    std::string vertexShader = m_shader.LoadShader("./shaders/gui_vert.glsl");
	std::string fragmentShader = m_shader.LoadShader("./shaders/gui_frag.glsl");
	// Actually create our shader
	m_shader.CreateShader(vertexShader,fragmentShader); 
    m_showOnScreen = true;
}

GUINode::~GUINode() {

}

void GUINode::Update() {
    m_shader.Bind();

    glm::mat4 projection = glm::ortho(0, 1, 1, 0, -100, 100);

    m_shader.SetUniformMatrix4fv("projection", &projection[0][0]);
    m_shader.SetUniformMatrix4fv("model", &m_transform.GetInternalMatrix()[0][0]);
}

void GUINode::Draw() {
    m_shader.Bind();

    if(m_object != nullptr && m_showOnScreen) {
        m_object->Render();
    }
}

void GUINode::PlaceOnScreen(int x, int y, int z) {

    float posX = (x * 1.0f) / (m_screenWidth * 1.0f);
    float posY = (y * 1.0f) / (m_screenHeight * 1.0f);

    m_transform.LoadIdentity();
    m_transform.Translate(posX, posY, 1.0f * z);
}

void GUINode::Resize(int width, int height) {
    float scaleX = width / (m_screenWidth * 1.0f);
    float scaleY = height / (m_screenHeight * 1.0f);
    m_transform.Scale(scaleX, scaleY, 1.0f);
}

void GUINode::SetScreenDimensions(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void GUINode::SetTextureFromLibrary(std::string textureName) {
    if(m_object != nullptr) {
        m_object->SetTextureFromLibrary(textureName);
    }
}