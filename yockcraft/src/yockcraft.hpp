#ifndef YOCKCRAFT_HPP
#define YOCKCRAFT_HPP

#include <SDL.h>
#undef main

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <stb/stb_image.h>

#include "defines.hpp"
// #include "utils.hpp"
// #include "rendering/vertex.hpp"
// #include "rendering/texture.hpp"
// #include "rendering/shader.hpp"
// #include "rendering/camera.hpp"
#include "rendering/window.hpp"
#include "rendering/renderer.hpp"
#include "world/terrain.hpp"

namespace yockcraft {

  struct State {
    bool running = true;
    bool camera_free = true;
    bool regenerate = false;
    RenderMode render_mode = RenderMode::FILLED; 
  };

  enum class HeightMapType {
    NOISE ,
    TERRAIN
  };

  struct NoiseData {
    HeightMapType map_type = HeightMapType::NOISE;
    int32_t seed = 0;
    float scale = 30.f;
    float persistence = 0.5f;
    float lacunarity = 2.f;
    int32_t num_octaves = 8;
    glm::vec2 offsets{ 0.f };
    std::vector<yockcraft::TerrainType> terrains{};
    std::array<float , yockcraft::world_area_bl> noise;
    std::array<uint8_t , yockcraft::world_area_bl> height_map;
    std::array<uint8_t , 4 * yockcraft::world_area_bl> terrain_pixels; // for texture
  };

  void PollEvents(Scope<Window>& window , State& state);

} // namespace yockcraft

#endif // !YOCKCRAFT_HPP
