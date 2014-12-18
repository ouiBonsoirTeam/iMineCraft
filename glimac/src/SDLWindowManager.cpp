#include "glimac/SDLWindowManager.hpp"
#include <iostream>

namespace glimac {

SDLWindowManager::SDLWindowManager(uint32_t width, uint32_t height, const char* title, bool fullscreen) {
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
<<<<<<< HEAD

    //antialiasing
    if (SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 ) == -1)
        std::cerr << "impossible d'initialiser SDL_GL_MULTISAMPLEBUFFERS à 1" ;
    else{
        if (SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 6 ) == -1)
            std::cerr << "impossible d'initialiser SDL_GL_MULTISAMPLESAMPLES sur 6 buffers";
        else
            std::cout << "anti aliasing démarré" << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);                                               
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,                
                width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!m_pWindow) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    m_Context = SDL_GL_CreateContext(m_pWindow);
    std::cerr << SDL_GetError() << std::endl;
=======
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
>>>>>>> camera
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

// uint32_t getWidth() const {
//     return m_width;
// }
// uint32_t getHeight() const {
//     return m_height;
// }

}
