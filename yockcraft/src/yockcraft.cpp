#include "yockcraft.hpp"

#include <SDL.h>

#include <imgui/backends/imgui_impl_sdl2.h>

namespace yockcraft {
  
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
          case SDLK_p: 
            if (state.render_mode == RenderMode::POINT) {
              state.render_mode = RenderMode::FILLED; 
            } else {
              state.render_mode = RenderMode::POINT; 
            }
          break;
          case SDLK_l: 
            if (state.render_mode == RenderMode::WIREFRAME) {
              state.render_mode = RenderMode::FILLED; 
            } else {
              state.render_mode = RenderMode::WIREFRAME; 
            }
          break;
          case SDLK_f: state.render_mode = RenderMode::FILLED; break;
          case SDLK_r: state.regenerate = true; break;
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
