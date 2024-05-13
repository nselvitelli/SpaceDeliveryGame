#include "render/KeyHandler.hpp"


#include <iostream>

KeyHandler& KeyHandler::GetInstance() {
    static KeyHandler handler;
    return handler;
}

KeyHandler::KeyHandler() {

}

KeyHandler::~KeyHandler() {
    
}

void KeyHandler::ProcessInputs(SDL_Event& e) {
    switch(e.type){
        // Handle keyboard presses
        case SDL_KEYDOWN:
            m_keysPressed.insert(e.key.keysym.sym);
            break;
        case SDL_KEYUP:
            m_keysPressed.erase(e.key.keysym.sym);
            break;
    }
}

bool KeyHandler::KeyPressed(SDL_Keycode key) {
    return m_keysPressed.count(key);
}