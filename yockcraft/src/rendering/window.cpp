/**
 * \file rendering/window.cpp
 **/
#include "rendering/window.hpp"

#include "utils.hpp"

namespace yockcraft {

  Scope<Window> GetWindow() {
    Scope<Window> window = std::make_unique<Window>();
  
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      utils::printerr("Could not initialize SDL");
      return nullptr;
    }
  
    window->window = SDL_CreateWindow(
      "YockCraft" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 
      window->size.x , window->size.y , window->flags
    );
    if (window == nullptr) {
      utils::printerr("SDL_CreateWindow Error: {}" , SDL_GetError());
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
      utils::printerr("SDL_GL_CreateContext Error: {}" , SDL_GetError());
      return nullptr;
    }
  
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
      utils::printerr("Failed to initialize GLAD");
      return nullptr;
    }
  
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
  
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
  
    return window;
  }
  
  void DestroyWindow(Scope<Window>& window) {
    if (window != nullptr) {
      SDL_GL_DeleteContext(window->context);
      SDL_DestroyWindow(window->window);
      SDL_Quit();
    }
  }

} // namespace yockcraft
