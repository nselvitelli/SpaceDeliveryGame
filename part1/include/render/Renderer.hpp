#ifndef RENDERER_HPP
#define RENDERER_HPP
/** @file Renderer.hpp
 *  @brief Renderer is responsible for drawing.
 *
 * 	Renderer is responsible for drawing everything. It
 *	contains a scenegraph node and a camera. We could 
 *	possibly have multiple renderers (if we had multiple
 *	windows for example).
 *
 *	Each renderer thus has it's own camera.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

// The glad library helps setup OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <glad/glad.h>

#include <vector>

#include "render/SceneNode.hpp"
#include "render/Camera.hpp"
#include "render/Skybox.hpp"
#include "render/GUI.hpp"

class Renderer{
public:
    // The constructor	
    // Sets the width and height of the rendererer draws to
    Renderer(unsigned int w, unsigned int h);
    // Destructor
    ~Renderer();
    // Update the scene
    void Update();
    // Render the scene
    void Render();
    // Sets the root of our renderer to some node to
    // draw an entire scene graph
    void setRoot(SceneNode* startingNode);
    // Returns the camera at an index
    Camera*& GetCamera(unsigned int index){
        if(index > m_cameras.size()-1){
            // TODO: Some sort of error reporting here.
        }
        return m_cameras[index];
    }

    void OverdriveFov(bool enable);

    inline void AddSkybox(Skybox* skybox) { m_skybox = skybox; }
    inline void AddGUI(GUI* gui) { m_gui = gui; }

    inline int Width() { return m_screenWidth; }
    inline int Height() { return m_screenHeight; }

    inline glm::mat4 ProjectionMatrix() { return m_projectionMatrix; }

// TODO: maybe write getter/setter methods
protected:
    // One or more cameras camera per Renderer
    std::vector<Camera*> m_cameras;
    // Root scene node
    SceneNode* m_root;
    // Store the projection matrix for our camera.
    glm::mat4 m_projectionMatrix;

private:
    // Screen dimension constants
    int m_screenHeight;
    int m_screenWidth;
    Skybox* m_skybox;
    GUI* m_gui;

    float m_fov;
    bool m_enableFov;
    const float m_fovTransitionSpeed{10.0f};
    const float m_normalFov{45.0f};
    const float m_overdriveFov{100.0f};

};

#endif
