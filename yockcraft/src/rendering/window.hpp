/**
 * \file rendering/window.hpp
 **/
#ifndef YOCKCRAFT_WINDOW_HPP
#define YOCKCRAFT_WINDOW_HPP

#include <SDL.h>
#undef main
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "defines.hpp"

namespace yockcraft {

  struct Window {
    SDL_Window* window;
    SDL_GLContext context;
  
    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    glm::ivec2 size{ 1920 , 1080 };
    glm::vec4 clear_color{ 0.2f , 0.3f , 0.3f , 1.0f };
  };
  
  Scope<Window> GetWindow();
  void DestroyWindow(Scope<Window>& window);

} // namespace yockcraft

#endif // !YOCKCRAFT_WINDOW_HPP
