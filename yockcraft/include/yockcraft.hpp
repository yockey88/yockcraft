#ifndef YOCKCRAFT_HPP
#define YOCKCRAFT_HPP

#include <iostream>

#include <SDL.h>
#undef main

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "defines.hpp"
#include "rendering/vertex.hpp"
#include "rendering/texture.hpp"
#include "rendering/shader.hpp"
#include "rendering/camera.hpp"
#include "world/chunk.hpp"


static const std::vector<std::string> cube_map_faces = {
    "yockcraft/resources/textures/right_grassblock2.png" ,
    "yockcraft/resources/textures/left_grassblock2.png" ,
    "yockcraft/resources/textures/top_grassblock2.png" ,
    "yockcraft/resources/textures/bottom_grassblock2.png" ,
    "yockcraft/resources/textures/front_grassblock2.png" ,
    "yockcraft/resources/textures/back_grassblock2.png"
};

namespace yockcraft {

    enum RenderMode {
        POINT = GL_POINT ,
        WIREFRAME = GL_LINE , 
        FILLED = GL_FILL
    };

    struct State {
        bool running = true;
        bool camera_free = true;
        RenderMode render_mode = RenderMode::FILLED; 
    };

    struct Window {
        SDL_Window* window;
        SDL_GLContext context;
    
        uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        glm::ivec2 size{ 1920 , 1080 };
        glm::vec4 clear_color{ 0.2f , 0.3f , 0.3f , 1.0f };
    };
    
    Scope<Window> GetWindow();
    void DestroyWindow(Scope<Window>& window);
    
    void PollEvents(Scope<Window>& window , State& state);

} // namespace yockcraft

#endif // !YOCKCRAFT_HPP
