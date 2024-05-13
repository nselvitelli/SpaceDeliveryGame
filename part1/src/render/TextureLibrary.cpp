#include "render/TextureLibrary.hpp"


#include <iostream>


TextureLibrary& TextureLibrary::GetInstance() {
    static TextureLibrary library;
    return library;
}

TextureLibrary::TextureLibrary() {

}

TextureLibrary::~TextureLibrary() {
    
}

bool TextureLibrary::HasTexture(std::string textureName) {
    return m_textureMap.count(textureName);
}


void TextureLibrary::LoadTexture(std::string filepath, std::string textureName, bool flip) {

    std::shared_ptr<Texture> texture_ptr(new Texture);
    texture_ptr->LoadTexture(filepath, flip);

    m_textureMap[textureName] = texture_ptr;
}

std::shared_ptr<Texture> TextureLibrary::GetTexture(std::string textureName) {
    if(HasTexture(textureName)) {
        return m_textureMap[textureName];
    }
    else {
        std::cout << "[Texture Library] Unable to find texture " << textureName << "... Creating default" << std::endl;
        std::shared_ptr<Texture> texture_ptr(new Texture);
        m_textureMap[textureName] = texture_ptr;
        return texture_ptr;

    }
}