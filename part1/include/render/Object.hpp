/** @file Object.hpp
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>
#include <memory>

#include "render/Shader.hpp"
#include "render/VertexBufferLayout.hpp"
#include "render/Texture.hpp"
#include "render/Transform.hpp"
#include "render/Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Load a texture
    void LoadTexture(std::string fileName);
    void SetTextureFromLibrary(std::string textureName);
    // Create a textured quad
    void MakeTexturedQuad(std::string textureName);
    // How to draw the object
    virtual void Render();
protected: // Classes that inherit from Object are intended to be overridden.

	// Helper method for when we are ready to draw or update our object
	void Bind();
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one diffuse map per object
    std::shared_ptr<Texture> m_textureDiffuse;
    // Store the objects Geometry
	Geometry m_geometry;
};


#endif
