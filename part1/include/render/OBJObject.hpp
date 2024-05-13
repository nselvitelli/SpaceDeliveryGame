#pragma once

#include "render/VertexBufferLayout.hpp"
#include "render/Geometry.hpp"
#include "render/Object.hpp"
#include "render/TextureLibrary.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

bool fSimilar(float a, float b) {
    return fabs(a - b) < 0.01f;
}

// vvvvvv Internal data structures to use within ObjLoader vvvvvv
struct Vertex {
    float x,y,z;    // position
    float nx, ny, nz; // normal
    float s, t; // texture coords

    Vertex(float _x, float _y, float _z, 
           float _nx, float _ny, float _nz,
           float _s, float _t): 
        x(_x), y(_y), z(_z), 
        nx(_nx), ny(_ny), nz(_nz),
        s(_s), t(_t)
        {}
    
    bool operator== (const Vertex &rhs) {
        return (fSimilar(x, rhs.x) && fSimilar(y, rhs.y) && fSimilar(z, rhs.z)
                && fSimilar(nx, rhs.nx) && fSimilar(ny, rhs.ny) && fSimilar(nz, rhs.nz)
                && fSimilar(s, rhs.s) && fSimilar(t, rhs.t));
    }
};

struct Face {
    int vertex_idxs[3];
    int texture_idxs[3];
    int normal_idxs[3];
};
// ^^^^^^^^^^^^^6


class OBJObject : public Object {
public:
    OBJObject(std::string objFilePath);

private:
    void LoadModelData(std::string objFilePath);
    bool loadBufferObjectsData(std::string filePath,
                               std::string& out_materialFilePath,
                               std::vector<float>& out_buffer,
                               std::vector<unsigned int>& out_indices);
    bool findVertexIndex(Vertex& vert,
                         std::vector<Vertex>& vertList,
                         int& out_index);
    void processToGLData(std::vector<glm::vec3> objVertices,
                         std::vector<glm::vec3> objNormals,
                         std::vector<glm::vec2> objTextureCoords,
                         std::vector<Face> objFaces,
                         std::vector<float>& out_buffer,
                         std::vector<unsigned int>& out_indices);
    bool loadMaterial(std::string filePath, 
                      std::string& out_diffuseTexturePath);
    
    std::string getFileFolderPath(std::string filepath);
};


OBJObject::OBJObject(std::string objFilePath) {
    LoadModelData(objFilePath);
    std::cout << "[OBJObject] loaded model " << objFilePath << std::endl;
}


void OBJObject::LoadModelData(std::string objFilePath) {
    std::string materialFilePath;
    std::vector<float> buffer;
    std::vector<unsigned int> indices;

    bool loaded = loadBufferObjectsData(objFilePath, materialFilePath, buffer, indices);

    if(!loaded) {
        std::cout << "[OBJObject] Unable to load " << objFilePath << std::endl;
        exit(1);
    }

    // 3. load texture diffuse map
    // 4. load texture normal map
    std::string diffuseTexturePath;
    loaded = loadMaterial(materialFilePath, diffuseTexturePath);

    if(!loaded) {
        std::cout << "[OBJObject] Unable to load Material " << materialFilePath << std::endl;
        exit(1);
    }

    m_vertexBufferLayout.CreateNormalBufferLayout(buffer.size(), indices.size(), buffer.data(), indices.data());
    
    std::string textureName = "Tex_" + objFilePath;
    if (!TextureLibrary::GetInstance().HasTexture(textureName)) {
        TextureLibrary::GetInstance().LoadTexture(diffuseTexturePath, textureName);
    }
    SetTextureFromLibrary(textureName);
}


bool OBJObject::loadBufferObjectsData(std::string filePath,
                               std::string& out_materialFilePath,
                               std::vector<float>& out_buffer,
                               std::vector<unsigned int>& out_indices) {
    
    std::vector<glm::vec3> objVertices;
    std::vector<glm::vec3> objNormals;
    std::vector<glm::vec2> objTextureCoords;
    std::vector<Face> objFaces;

    std::ifstream inputFile;
    inputFile.open(filePath);

    if(inputFile.is_open()) {
        std::string line;

        while(std::getline(inputFile, line)) {
            
            std::stringstream stream(line);
            

            std::string token;
            stream >> token;

            if(token == "v") {

                glm::vec3 v;

                stream >> token;
                v.x = std::stof(token);
                stream >> token;
                v.y = std::stof(token);
                stream >> token;
                v.z = std::stof(token);
                
                objVertices.push_back(v);
            }
            else if(token == "vn") {
                
                glm::vec3 n;

                stream >> token;
                n.x = std::stof(token);
                stream >> token;
                n.y = std::stof(token);
                stream >> token;
                n.z = std::stof(token);

                objNormals.push_back(n);
            }
            else if(token == "vt") {
                glm::vec2 t;

                stream >> token;
                t.x = std::stof(token);
                stream >> token;
                t.y = 1.0f - std::stof(token);

                objTextureCoords.push_back(t);
            }
            else if(token == "f") {
                
                Face f;

                stream >> token;
                size_t pos = token.find("/");
                f.vertex_idxs[0] = std::stoi(token.substr(0, pos)) - 1;
                
                token = token.substr(pos+1);
                pos = token.find("/");
                f.texture_idxs[0] = std::stoi(token.substr(0, pos)) - 1;
                f.normal_idxs[0] = std::stoi(token.substr(pos+1)) - 1;


                stream >> token;
                pos = token.find("/");
                f.vertex_idxs[1] = std::stoi(token.substr(0, pos)) - 1;
                
                token = token.substr(pos+1);
                pos = token.find("/");
                f.texture_idxs[1] = std::stoi(token.substr(0, pos)) - 1;
                f.normal_idxs[1] = std::stoi(token.substr(pos+1)) - 1;


                stream >> token;
                pos = token.find("/");
                f.vertex_idxs[2] = std::stoi(token.substr(0, pos)) - 1;
                
                token = token.substr(pos+1);
                pos = token.find("/");
                f.texture_idxs[2] = std::stoi(token.substr(0, pos)) - 1;
                f.normal_idxs[2] = std::stoi(token.substr(pos+1)) - 1;


                objFaces.push_back(f);
            }
            else if(token == "mtllib") {
                stream >> token;

                std::string prefix = getFileFolderPath(filePath);

                out_materialFilePath = (prefix + token);
            }
        }
        
        processToGLData(objVertices,
                        objNormals,
                        objTextureCoords,
                        objFaces,
                        out_buffer,
                        out_indices);

        inputFile.close();
        return true;
    }
    else {
        return false;
    }
}


std::string OBJObject::getFileFolderPath(std::string filepath) {
    std::size_t found = filepath.rfind("/");
    if(found != std::string::npos) {
        return filepath.substr(0, found+1);
    }
    return filepath;
}


bool OBJObject::findVertexIndex(Vertex & vert,
                     std::vector<Vertex> & vertList,
                     int & out_index) {
    
    out_index = -1;
    for(int i = 0; i < vertList.size(); i++) {
        if(vert == vertList[i]) {
            out_index = i;
            return true;
        }
    }
    return false;
}


void OBJObject::processToGLData(std::vector<glm::vec3> objVertices,
                                std::vector<glm::vec3> objNormals,
                                std::vector<glm::vec2> objTextureCoords,
                                std::vector<Face> objFaces,
                                std::vector<float> & out_buffer,
                                std::vector<unsigned int> & out_indices) {

    std::vector<Vertex> vertices;

    for(int i = 0; i < objFaces.size(); i++) {
        Face f = objFaces[i];

        // Compute Tangent and Bi-Tangent for face vertices
        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = objVertices[f.vertex_idxs[1]] - objVertices[f.vertex_idxs[0]];
        glm::vec3 deltaPos2 = objVertices[f.vertex_idxs[2]] - objVertices[f.vertex_idxs[0]];

        // UV delta
        glm::vec2 deltaUV1 = objTextureCoords[f.texture_idxs[1]] - objTextureCoords[f.texture_idxs[0]];
        glm::vec2 deltaUV2 = objTextureCoords[f.texture_idxs[2]] - objTextureCoords[f.texture_idxs[0]];

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

        for (int b = 0; b < 3; b++) {
            int vertIdx = f.vertex_idxs[b];
            int normIdx = f.normal_idxs[b];
            int textIdx = f.texture_idxs[b];

            glm::vec3 pos = objVertices[vertIdx];
            glm::vec3 norm = objNormals[normIdx];
            glm::vec2 tex = objTextureCoords[textIdx];

            Vertex vertex = Vertex(pos.x, pos.y, pos.z, 
                                   norm.x, norm.y, norm.z,
                                   tex.x, tex.y);

            int index;
            bool found = findVertexIndex(vertex, vertices, index);      
            
            if(found) { //vertex exists
                out_indices.push_back(index);
                m_geometry.AddIndex(index);
            }
            else {
                
                out_buffer.push_back(pos.x);
                out_buffer.push_back(pos.y);
                out_buffer.push_back(pos.z);

                out_buffer.push_back(norm.x);
                out_buffer.push_back(norm.y);
                out_buffer.push_back(norm.z);

                out_buffer.push_back(tex.x);
                out_buffer.push_back(tex.y);

                out_buffer.push_back(tangent.x);
                out_buffer.push_back(tangent.y);
                out_buffer.push_back(tangent.z);

                out_buffer.push_back(bitangent.x);
                out_buffer.push_back(bitangent.y);
                out_buffer.push_back(bitangent.z);

                vertices.push_back(vertex);

                int newIndex = vertices.size() - 1;
                out_indices.push_back(newIndex);

                // hacky way to get index size in Geometry class
                // TODO: fix obj loader to work with Geometry class
                m_geometry.AddVertex(pos.x, pos.y, pos.z, tex.s, tex.t);
                m_geometry.AddIndex(newIndex); 
            }
        }
    }
}




bool OBJObject::loadMaterial(std::string filePath, 
                    std::string & out_diffuseTexturePath) {

    std::string prefix = getFileFolderPath(filePath);

    std::ifstream inputFile;
    inputFile.open(filePath);

    if(inputFile.is_open()) {
        std::string line;

        while(std::getline(inputFile, line)) {
            std::stringstream stream(line);
            
            std::string token;
            
            stream >> token;

            if(token  == "map_Kd") {
                stream >> token;
                out_diffuseTexturePath = (prefix + token);
            }
        }
        inputFile.close();
        return true;
    }
    else {
        return false;
    }
}

