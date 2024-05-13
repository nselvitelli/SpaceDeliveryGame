#include "render/Renderer.hpp"

#include <iostream>

// Sets the height and width of our renderer
Renderer::Renderer(unsigned int w, unsigned int h){
    m_screenWidth = w;
    m_screenHeight = h;

    // By default create one camera per render
    // TODO: You could abstract out further functions to create
    //       a camera as a scene node and attach them at various levels.
    Camera* defaultCamera = new Camera();
    // Add our single camera
    m_cameras.push_back(defaultCamera);

    m_fov = m_normalFov;

    m_root = nullptr;
    m_skybox = nullptr;
    m_gui = nullptr;
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
    // Delete all of our camera pointers
    for(int i=0; i < m_cameras.size(); i++){
        delete m_cameras[i];
    }
}

void Renderer::Update(){
    // Here we apply the projection matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane.
    // Note I cannot see anything closer than 0.1f units from the screen.
    float farClip = 1000.0f; //512.0f;
    m_projectionMatrix = glm::perspective(glm::radians(m_fov),((float)m_screenWidth)/((float)m_screenHeight), 0.1f, farClip);

    if(m_enableFov) {
        float totalDist = m_overdriveFov - m_normalFov;
        float distTaken = m_fov - m_normalFov;
        float percentDone = distTaken / totalDist;

        // easing function to make speed up look cooler
        // f(x) = x^3 + 0.1
        float speed = std::pow(percentDone, 3) + 0.1f;

        m_fov = std::fmin(m_overdriveFov, m_fov + (speed * m_fovTransitionSpeed));
    }
    else {
        float totalDist = m_overdriveFov - m_normalFov;
        float distTaken = m_overdriveFov - m_fov;
        float percentDone = distTaken / totalDist;

        // f(x) = -(x - 1)^3 + 0.1
        float speed = (-1.0f * std::pow(percentDone - 1.0f, 3)) + 0.1f;

        m_fov = std::fmax(m_normalFov, m_fov - (speed * m_fovTransitionSpeed));
    }

    // Perform the update
    if(m_root!=nullptr){
        // Only using one camera so pass in first
        m_root->Update(m_projectionMatrix, m_cameras[0]);
    }

    if(m_skybox != nullptr) {
        m_skybox->Update(m_projectionMatrix, m_cameras[0]);
    }

    if(m_gui != nullptr) {
        m_gui->Update();
    }
}

// Initialize clear color
// Setup our OpenGL State machine
// Then render the scene
void Renderer::Render(){

    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
    glEnable(GL_BLEND); // alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.01f, 0.01f, 0.01f, 1.f );
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Draw all SceneNodes in wireframe
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    
    // Now we render our objects from our scenegraph
    if(m_root!=nullptr){
        m_root->Draw();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(m_skybox != nullptr) {
        m_skybox->Draw();
    }

    // GUI requires no depth test
    glDisable(GL_DEPTH_TEST);
    if(m_gui != nullptr) {
        m_gui->Draw();
    }
}

// Determines what the root is of the renderer, so the
// scene can be drawn.
void Renderer::setRoot(SceneNode* startingNode){
    m_root = startingNode;
}


void Renderer::OverdriveFov(bool enable) {
    m_enableFov = enable;
}