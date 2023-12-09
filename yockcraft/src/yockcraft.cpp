#include "yockcraft.hpp"

#include <iostream>

namespace yockcraft {

    Scope<Window> GetWindow() {
        Scope<Window> window = std::make_unique<Window>();
    
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }
    
        window->window = SDL_CreateWindow(
            "YockCraft" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 
            window->size.x , window->size.y , window->flags
        );
        if (window == nullptr) {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_SetRelativeMouseMode(SDL_TRUE);
    
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION , 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION , 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER , 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE , 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE , 8);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL , 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS , 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES , 16);
        SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE , 1);
    
        window->context = SDL_GL_CreateContext(window->window);
        if (window->window == nullptr) {
            std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }
    
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }
    
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    
        // glEnable(GL_MULTISAMPLE);
       
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    
        return window;
    }
    
    void DestroyWindow(Scope<Window>& window) {
        if (window != nullptr) {
            SDL_GL_DeleteContext(window->context);
            SDL_DestroyWindow(window->window);
            SDL_Quit();
        }
    }
    
    void PollEvents(Scope<Window>& window , State& state) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        state.running = false;
                    break;
                    case SDL_WINDOWEVENT_RESIZED:
                        window->size.x = event.window.data1; window->size.y = event.window.data2;
                        glViewport(0 , 0 , window->size.x , window->size.y);
                    break; 
                    default: break;
                }
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_c:
                        state.camera_free = !state.camera_free;
                        if (state.camera_free) {
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                        } else {
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                        }
                    break;
                    case SDLK_p: state.render_mode = RenderMode::POINT; break;
                    case SDLK_l: state.render_mode = RenderMode::WIREFRAME; break;
                    case SDLK_f: state.render_mode = RenderMode::FILLED; break;
                    case SDLK_ESCAPE: 
                        state.running = false;
                    break;
                    default: break;
                }
            }
            if (event.type == SDL_QUIT) {
                state.running = false;
            } 

            ImGui_ImplSDL2_ProcessEvent(&event);
        }
    }

} // namespace yockcraft
