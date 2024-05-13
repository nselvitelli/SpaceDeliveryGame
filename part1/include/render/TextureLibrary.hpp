#pragma once

#include <string>
#include <memory>
#include <map>

#include "render/Texture.hpp"

/**
 * Hold all textures used in the project within a 
 * library so each unique texture is only loaded once.
*/
class TextureLibrary {
public:
    static TextureLibrary& GetInstance();
    ~TextureLibrary();

    bool HasTexture(std::string textureName);
    void LoadTexture(std::string filepath, std::string textureName, bool flip = true);
    std::shared_ptr<Texture> GetTexture(std::string textureName);

private:
    TextureLibrary();
    std::map<std::string, std::shared_ptr<Texture>> m_textureMap;
};