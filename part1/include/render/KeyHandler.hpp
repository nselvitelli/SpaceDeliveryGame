#pragma once

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <set>

class KeyHandler {
public:
    static KeyHandler& GetInstance();
    ~KeyHandler();

    void ProcessInputs(SDL_Event& e);

    bool KeyPressed(SDL_Keycode key);

private:
    KeyHandler();

    std::set<SDL_Keycode> m_keysPressed;
};