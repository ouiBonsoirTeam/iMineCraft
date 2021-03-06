#include "glimac/SDLWindowManager.hpp"
#include <iostream>

namespace glimac {

SDLWindowManager::SDLWindowManager(const char* title, bool fullscreen) {
    uint32_t width = 0;
    uint32_t height = 0;
    SDL_DisplayMode current;

    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }

    int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);
    if (should_be_zero !=0)
    {
        std::cerr << SDL_GetError() << std::endl;
    }
    else{
        std::cout << "User screen resolution : " << current.w << "px * " << current.h << "px" << std::endl;
        width = current.w;
        height = current.h;
    }

    //antialiasing
    if (SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 ) == -1)
        std::cerr << "impossible d'initialiser SDL_GL_MULTISAMPLEBUFFERS à 1" ;
    else{
        if (SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 6 ) == -1)
            std::cerr << "impossible d'initialiser SDL_GL_MULTISAMPLESAMPLES sur 6 buffers";
        else
            std::cout << "anti aliasing démarré" << std::endl;
    }

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);                                               
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    

    if(fullscreen)
    {
        m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,                
                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
        if(!m_pWindow) {
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
    }
    else
    {
        m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,                
                800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if(!m_pWindow) {
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
    }
    //std::cout << "Depuis SDLWM" << m_pWindow->w << std::endl;

    m_Context = SDL_GL_CreateContext(m_pWindow);
    std::cerr << SDL_GetError() << std::endl;

    //avec SDL2
    SDL_SetRelativeMouseMode(SDL_bool(true));

}

SDLWindowManager::~SDLWindowManager() {
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}

bool SDLWindowManager::pollEvent(SDL_Event& e) {
    return SDL_PollEvent(&e);
}

bool SDLWindowManager::isKeyPressed(SDL_Keycode key) const {
    return SDL_GetKeyboardState(nullptr)[SDL_GetScancodeFromKey(key)];
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
    SDL_GL_SwapWindow(m_pWindow);
}

float SDLWindowManager::getTime() const {
    return 0.001f * SDL_GetTicks();
}

SDL_Window* SDLWindowManager::getWindow() const {
    return m_pWindow;
}

}
