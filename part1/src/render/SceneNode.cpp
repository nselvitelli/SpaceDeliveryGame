#include "render/SceneNode.hpp"

#include <string>
#include <iostream>

// The constructor
SceneNode::SceneNode(Object* ob){
	m_object = ob;
	// By default, we do not know the parent
	// at the time of construction of this node.
	// If the SceneNode is the root of the tree,
	// then there is no parent.
	m_parent = nullptr;
	
	m_drawColor = glm::vec3(0.0f, 1.0f, 0.0f);

	// Setup shaders for the node.
	std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
	std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
	// Actually create our shader
	m_shader.CreateShader(vertexShader,fragmentShader);       
}

// The destructor 
SceneNode::~SceneNode(){
	// Remove each object
	for(unsigned int i =0; i < m_children.size(); ++i){
		delete m_children[i];
	}
}

// Adds a child node to our current node.
void SceneNode::AddChild(SceneNode* n){
	// For the node we have added, we can set
	// it's parent now to our current node.
	// 'this' is the current instance of our
	// object, which is a pointer to our current
	// SceneNode.
	n->m_parent = this;
	// Add a child node into our SceneNode
	m_children.push_back(n);
}

// Draw simply draws the current nodes
// object and all of its children. This is done by calling directly
// the objects draw method.
void SceneNode::Draw(){
	// Bind the shader for this node or series of nodes
	m_shader.Bind();
	// Render our object
	if(m_object!=nullptr){
		// Render our object
		m_object->Render();
	}	
	// For any 'child nodes' also call the drawing routine.
	for(int i =0; i < m_children.size(); ++i){
		m_children[i]->Draw();
	}
}

// Update simply updates the current nodes
// object. This is done by calling directly
// the objects update method.
// TODO: Consider not passting projection and camera here
void SceneNode::Update(glm::mat4 projectionMatrix, Camera* camera){

	if(m_parent != nullptr) {
		m_worldTransform = m_parent->m_worldTransform * m_localTransform;
	}
	else {
		m_worldTransform = m_localTransform;
	}

	// Now apply our shader 
	m_shader.Bind();
	// Set the uniforms in our current shader

	// Set the MVP Matrix for our object
	// Send it into our shader
	m_shader.SetUniformMatrix4fv("model", &m_worldTransform.GetInternalMatrix()[0][0]);

	glm::mat4 view;
	if(!m_lockObjectView) {
		view = camera->GetWorldToViewmatrix();
	}

	m_shader.SetUniformMatrix4fv("view", &view[0][0]);
	m_shader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

	m_shader.SetUniform3f("u_color", m_drawColor.r, m_drawColor.g, m_drawColor.b);

	// Iterate through all of the children
	for(int i =0; i < m_children.size(); ++i){
		m_children[i]->Update(projectionMatrix, camera);
	}
}

// Returns the actual local transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::GetLocalTransform(){
    return m_localTransform; 
}

// Returns the worled  transform stored in our SceneNode
// which can then be modified
Transform& SceneNode::GetWorldTransform(){
    return m_worldTransform; 
}
