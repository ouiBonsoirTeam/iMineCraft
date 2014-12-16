#include "glimac/SDLWindowManager.hpp"
#include <iostream>

namespace glimac {

SDLWindowManager::SDLWindowManager(uint32_t width, uint32_t height, const char* title, bool fullscreen) {
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    if (fullscreen == 1)
    {
        const SDL_VideoInfo* myPointer = SDL_GetVideoInfo();

        std::cout << "Current video resolution is " << myPointer->current_w << "x" << myPointer->current_h << " pixels" << std::endl;
        
        if(!SDL_SetVideoMode(myPointer->current_w, myPointer->current_h, 32, SDL_OPENGL | SDL_FULLSCREEN)) {
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
    }
    else 
    {
        if(!SDL_SetVideoMode(width, height, 32, SDL_OPENGL)) {
            std::cerr << SDL_GetError() << std::endl;
            return;
        }

    }

    SDL_ShowCursor(0);


    //avec SDL2
    //SDL_SetRelativeMouseMode(true);

    //avec SDL1.2
    SDL_WM_GrabInput(SDL_GRAB_ON);

    //m_width = width;
    //m_height = height;
    
    SDL_WM_SetCaption(title, nullptr);
}

SDLWindowManager::~SDLWindowManager() {
    SDL_Quit();
}

bool SDLWindowManager::pollEvent(SDL_Event& e) {
    return SDL_PollEvent(&e);
}

bool SDLWindowManager::isKeyPressed(SDLKey key) const {
    return SDL_GetKeyState(nullptr)[key];
}

// button can SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE
bool SDLWindowManager::isMouseButtonPressed(uint32_t button) const {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

glm::ivec2 SDLWindowManager::getMousePosition() const {
    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return mousePos;
}

void SDLWindowManager::swapBuffers() {
    SDL_GL_SwapBuffers();
}

float SDLWindowManager::getTime() const {
    return 0.001f * SDL_GetTicks();
}

// uint32_t getWidth() const {
//     return m_width;
// }
// uint32_t getHeight() const {
//     return m_height;
// }

}
