#include <chrono>
#include <imgui/imgui.h>

#include <filesystem>
#include <glm/common.hpp>
#include <glm/ext/scalar_common.hpp>
#include <limits>
#include <random>
#include <rendering/renderer.hpp>

#include "utils.hpp"
#include "yockcraft.hpp"
#include "world/terrain.hpp"
#include "world/chunk.hpp"
#include "rendering/color.hpp"

/**
 * This example is poorly formed at the moment because it was the beginning of the project
 *  where the initial foundations were laid down on top of some pre existing opengl wrappers, it
 *  is slow and the terrain gen is weird because the terrain gen (while it works) isnt tuned yet 
 *  so it is a bit extreme
 **/

void CameraKeyboardCallback(yockcraft::Camera* camera);
void CameraMouseCallback(yockcraft::Camera* camera);

static const std::vector<float> xaxis{
  -(float)yockcraft::half_world_dim_bl , 0.f , 0.f ,
   (float)yockcraft::half_world_dim_bl , 0.f , 0.f ,
};

static const std::vector<float> yaxis{
  0.f , -(float)yockcraft::half_world_dim_bl , 0.f ,
  0.f ,  (float)yockcraft::half_world_dim_bl , 0.f ,
};

static const std::vector<float> zaxis{
  0.f , 0.f , -(float)yockcraft::half_world_dim_bl ,
  0.f , 0.f ,  (float)yockcraft::half_world_dim_bl ,
};

void GenerateNoiseMap(yockcraft::NoiseData* noise_data);
void GenerateHeightMap(yockcraft::NoiseData* noise_data);
void GenerateTerrain(yockcraft::NoiseData* noise_data);

void DrawLineMesh(yockcraft::Shader& shader , yockcraft::VertexArray& mesh , const glm::vec3& line_color) {
  shader.SetUniform("model", glm::mat4(1.0f));
  shader.SetUniform("uni_color", line_color);
  mesh.Draw(yockcraft::DrawMode::LINES);
}

void DrawCameraControls(yockcraft::Camera& camera);
void DrawTerrainTypeEditor(yockcraft::TerrainType& terr_type);

int main(int argc, char* argv[]) {
  /// this is for debug reasons Visual Studio debugger opens it in different cwd
  ///   and I'm too lazy to find out how to change working directory through premake generation
  if (argc > 1) {
    std::filesystem::current_path(std::filesystem::path("../../../"));
  }
  yockcraft::State state;

  yockcraft::RenderState* renderer = yockcraft::Renderer::Initialize({});

  {
    yockcraft::VertexArray xaxis_vao(xaxis);
    yockcraft::VertexArray yaxis_vao(yaxis);
    yockcraft::VertexArray zaxis_vao(zaxis);

    yockcraft::Shader shader("yockcraft/resources/shaders/shader.vert" , "yockcraft/resources/shaders/shader.frag");
    if (!shader.IsValid()) {
      std::cout << "Failed to create shader" << std::endl;
      yockcraft::Renderer::Shutdown();
      return 1;
    }
    
    yockcraft::Shader norm_shader("yockcraft/resources/shaders/normals.vert" , "yockcraft/resources/shaders/normals.frag" ,
                                  "yockcraft/resources/shaders/normals.geom");
    if (!norm_shader.IsValid()) {
      std::cout << "Failed to create normal shader" << std::endl;
      yockcraft::Renderer::Shutdown();
      return 1;
    }

    float normal_magnitude = 1.f;

    yockcraft::Shader line_shader("yockcraft/resources/shaders/line.vert" , "yockcraft/resources/shaders/line.frag");
    if (!line_shader.IsValid()) {
      std::cout << "Failed to create line shader" << std::endl;
      yockcraft::Renderer::Shutdown();
      return 1;
    }

    yockcraft::Texture atlas("yockcraft/resources/textures/blocks0.png");

    int32_t img_scale = 3;
    const size_t hmap_w = yockcraft::world_dim_bl;
    const size_t hmap_h = yockcraft::world_dim_bl;

    yockcraft::NoiseData* noise_data = new yockcraft::NoiseData{};
    noise_data->terrains = {
      {
        .name = "water",
        .height_max = 0.3f,
        .rgb = {
          16.f / 255,
          36.f / 255,
          224.f / 255,
        }
      } , 
      {
        .name = "grass" ,
        .height_max = 0.5f ,
        .rgb = {
          23.f / 255 ,
          64.f / 255 ,
          3.f / 255 ,
        }
      } , 
      {
        .name = "rock" ,
        .height_max = 0.7f ,
        .rgb = {
          42.f / 255 ,
          45.f / 255 ,
          40.f / 255 ,
        }
      } , 
      {
        .name = "snow" ,
        .height_max = 1.f ,
        .rgb = {
          1.f ,
          1.f ,
          1.f ,
        }
      }
    };

    bool hmap_noise = true;
    bool hmap_terrain = false;

    GenerateTerrain(noise_data);
    
    std::chrono::time_point<std::chrono::system_clock> now, last;

    yockcraft::Terrain terrain{ 0  , noise_data->height_map };
    Scope<yockcraft::Texture> hmap = NewScope<yockcraft::Texture>(noise_data->terrain_pixels.data() , 
                                                                  glm::ivec2{ yockcraft::world_dim_bl , yockcraft::world_dim_bl });

    std::array<Scope<yockcraft::Chunk> , yockcraft::world_area> chunks = { nullptr };
    uint32_t chunk_index = 0;
    for (int32_t z = -yockcraft::shalf_world_dim_bl; z < yockcraft::shalf_world_dim_bl; z += yockcraft::schunk_dim) {
      for (int32_t x = -yockcraft::shalf_world_dim_bl; x < yockcraft::shalf_world_dim_bl; x += yockcraft::schunk_dim) {
        chunks[chunk_index] = NewScope<yockcraft::Chunk>(glm::vec3(x, 0.0f, z), terrain);
        ++chunk_index;
      }
    }

    yockcraft::Camera camera;
    camera.SetPosition(glm::vec3(0.0f, (float)yockcraft::chunk_height / 2, 3.0f));
    camera.SetSpeed(1.5f);


    yockcraft::NoiseData* last_noise_data = new yockcraft::NoiseData{};

    now = std::chrono::system_clock::now();
    while (state.running) {
      last = now;
      now = std::chrono::system_clock::now();
      std::chrono::duration<float> delta = now - last;

      last_noise_data = noise_data;

      PollEvents(renderer->window, state);

      if (state.camera_free) {
        CameraKeyboardCallback(&camera);
        CameraMouseCallback(&camera);
      }

      if (state.regenerate) {
        GenerateTerrain(noise_data);

        terrain = { 0 , noise_data->height_map };
        hmap = NewScope<yockcraft::Texture>(noise_data->terrain_pixels.data() , glm::ivec2{ yockcraft::world_dim_bl , yockcraft::world_dim_bl });

        uint32_t chunk_index = 0;
        for (int32_t z = -yockcraft::shalf_world_dim_bl; z < yockcraft::shalf_world_dim_bl; z += yockcraft::schunk_dim) {
          for (int32_t x = -yockcraft::shalf_world_dim_bl; x < yockcraft::shalf_world_dim_bl; x += yockcraft::schunk_dim) {
            chunks[chunk_index] = NewScope<yockcraft::Chunk>(glm::vec3(x, 0.0f, z), terrain);
            ++chunk_index;
          }
        }

        state.regenerate = false;
      }

      glPolygonMode(GL_FRONT_AND_BACK, state.render_mode);

      yockcraft::Renderer::BeginRender();
      yockcraft::Renderer::BeginUIRender();

      atlas.Bind();
      shader.Bind();
      shader.SetUniform("projection", camera.Projection());
      shader.SetUniform("view", camera.View());
      for (auto& ch : chunks) {
        if (ch == nullptr) {
          continue;
        }

        ch->Draw(&shader);
      }
      atlas.Unbind();

      norm_shader.Bind();
      norm_shader.SetUniform("projection", camera.Projection());
      norm_shader.SetUniform("view", camera.View());
      norm_shader.SetUniform("magnitude" , normal_magnitude);
      for (auto& ch : chunks) {
        if (ch == nullptr) {
          continue;
        }

        ch->Draw(&norm_shader);
      }


      line_shader.Bind();
      line_shader.SetUniform("projection", camera.Projection());
      line_shader.SetUniform("view", camera.View());

      /// main x-axis
      DrawLineMesh(line_shader , xaxis_vao , { 1.f , 0.f , 0.f });
      DrawLineMesh(line_shader , yaxis_vao , { 0.f , 1.f , 0.f });
      DrawLineMesh(line_shader , zaxis_vao , { 0.f , 0.f , 1.f });

      /// other x-axes
      line_shader.SetUniform("uni_color", glm::vec3(0.37f));
      for (int z = -64; z <= 64; ++z) {
        if (z == 0) {
          continue;
        }
        line_shader.SetUniform("model" , glm::translate(glm::mat4(1.f), {0, 0, z}));
        xaxis_vao.Draw(yockcraft::DrawMode::LINES);
      }

      /// other z-axes
      line_shader.SetUniform("uni_color", glm::vec3(0.37f));
      for (int x = -64; x <= 64; ++x) {
        if (x == 0) {
          continue;
        }
        line_shader.SetUniform("model" , glm::translate(glm::mat4(1.f), {x, 0, 0}));
        zaxis_vao.Draw(yockcraft::DrawMode::LINES);
      }

      if (ImGui::Begin("Scene Data")) {
        ImGui::Text("FPS :: [%f]", 1.0f / delta.count());
        ImGui::Separator();
        ImGui::Separator();

        DrawCameraControls(camera);

        ImGui::Separator(); 
        ImGui::Separator(); 

        ImGui::SliderFloat("Normals Length" , &normal_magnitude , 0.1f , 3.f);
        
        ImGui::Separator(); 
        ImGui::Separator(); 

        if (ImGui::BeginChild("Textures")) {
          ImGui::Text("[ Block Atlas ]");
          ImGui::Image((void*)(intptr_t)atlas.ID(), ImVec2(512, 256),
                       ImVec2(0, 1), ImVec2(1, 0));

          ImGui::Separator();
          ImGui::Separator();

          ImGui::SliderInt("Image Scale", &img_scale, 1, 10);

          ImGui::InputInt("Seed", &noise_data->seed);
          ImGui::SliderFloat("Scale", &noise_data->scale, 1.f, 100.f);
          ImGui::SliderFloat("Persistence", &noise_data->persistence, 0.f, 1.f);
          ImGui::SliderFloat("Lacunarity", &noise_data->lacunarity, 1.f, 5.f);
          ImGui::SliderInt("Num Octaves", &noise_data->num_octaves, 1, 8);

          for (uint32_t i = 0; i < noise_data->terrains.size(); ++i) {
            ImGui::PushID(i);
            DrawTerrainTypeEditor(noise_data->terrains[i]);
            ImGui::PopID();
          }

          float os[2] = { noise_data->offsets.x , noise_data->offsets.y };
          ImGui::SliderFloat2("Offsets", os, -100.f, 100.f);
          noise_data->offsets = { os[0] , os[1] };

          if (last_noise_data->seed != noise_data->seed               || 
              last_noise_data->scale != noise_data->scale             ||
              last_noise_data->persistence != noise_data->persistence ||
              last_noise_data->lacunarity != noise_data->lacunarity   ||
              last_noise_data->num_octaves != noise_data->num_octaves || 
              last_noise_data->offsets.x != noise_data->offsets.x     ||
              last_noise_data->offsets.y != noise_data->offsets.y) {
            state.regenerate = true;
          }

          bool hmn = hmap_noise;
          if (ImGui::Checkbox("Noise Map" , &hmap_noise)) {
            if (hmap_noise != hmn) {
              noise_data->map_type = yockcraft::HeightMapType::NOISE;
              hmap_terrain = false;
              state.regenerate = true;
            }
          } 

          bool hmt = hmap_terrain;
          if (ImGui::Checkbox("Terrain Map" , &hmap_terrain)) {
            if (hmap_terrain != hmt) {
              noise_data->map_type = yockcraft::HeightMapType::TERRAIN;
              hmap_noise = false;
              state.regenerate = true;
            }
          }

          if (hmap != nullptr) {
            ImGui::Text("[ Height Map ]");

            if (ImGui::Button("[ Regenerate ]")) {
              state.regenerate = true;
            }

            ImVec2 raw_dim{ hmap_w , hmap_h };
            ImVec2 tex_dim{ raw_dim.x * img_scale , raw_dim.y * img_scale };
            ImGui::Image((void*)(intptr_t)hmap->ID() , tex_dim);
            ImGui::Separator();
          }
        }
        ImGui::EndChild();
      }
      ImGui::End();

      if (ImGui::Begin("Debug Controls")) {
        if (ImGui::BeginChild("Camera Controls")) {
          float speed = camera.Speed();
          ImGui::DragFloat("Camera Speed", &speed, 0.1f, 0.0f, 100.0f);
          camera.SetSpeed(speed);
        }
        ImGui::EndChild();
      }
      ImGui::End();

      yockcraft::Renderer::FinishUIRender();
      yockcraft::Renderer::EndRender();
      yockcraft::Renderer::SwapBuffers();
    }
  }

  yockcraft::Renderer::Shutdown();

  std::cout << "YockCraft Exit Validated" << std::endl;
  return 0;
}

void GenerateNoiseMap(yockcraft::NoiseData* noise_data) {
  std::mt19937 mersenne(noise_data->seed);
  std::uniform_real_distribution<float> dist(-100000.f, 100000.f);

  std::vector<glm::vec2> offsets = {};
  offsets.resize(noise_data->num_octaves);
  for (size_t i = 0; i < noise_data->num_octaves; ++i) {
    float offset_x = dist(mersenne) + noise_data->offsets.x;
    float offset_z = dist(mersenne) + noise_data->offsets.y;

    offsets[i] = { offset_x , offset_z };
  }

#undef min
#undef max

  float min = std::numeric_limits<float>::max();
  float max = std::numeric_limits<float>::min();

  for (float z = 0.f; z < yockcraft::world_dim_bl; ++z) {
    for (float x = 0.f; x < yockcraft::world_dim_bl; ++x) {
      size_t idx = (size_t)(x + z * yockcraft::world_dim_bl);

      float amp = 1.f;
      float freq = 1.f;

      float h = 0.f;

      for (int32_t i = 0; i < noise_data->num_octaves; ++i) {
        glm::vec2 in = { 
          (x - yockcraft::half_world_dim_bl) / noise_data->scale * freq + offsets[i].x ,
          (z - yockcraft::half_world_dim_bl) / noise_data->scale * freq + offsets[i].y 
        };

        float pval = glm::perlin(in) * 2.f + 1.f;
        h += pval * amp;

        amp *= noise_data->persistence;
        freq *= noise_data->lacunarity;
      }

      noise_data->noise[idx] = h;

      if (h < min) {
        min = h;
      } else if (h > max) {
        max = h;
      }
    }
  }

  for (size_t z = 0; z < yockcraft::world_dim_bl; ++z) {
    for (size_t x = 0; x < yockcraft::world_dim_bl; ++x) {
      size_t idx = x + z * yockcraft::world_dim_bl;
      noise_data->noise[idx] = yockcraft::InverseLerp(min, max, noise_data->noise[idx]);
      noise_data->height_map[idx] = (noise_data->noise[idx] >= 1.f) ? 
        yockcraft::chunk_height : noise_data->noise[idx] * yockcraft::chunk_height;
      noise_data->height_map[idx] = glm::max<uint8_t>(230u , noise_data->height_map[idx]);
    }
  }
}

void GenerateHeightMap(yockcraft::NoiseData* noise_data) {
  yockcraft::HeightMapType type = noise_data->map_type;
  if (type == yockcraft::HeightMapType::TERRAIN && noise_data->terrains.size() == 0) {
    type = yockcraft::HeightMapType::NOISE;
  }

  for (size_t z = 0; z < yockcraft::world_dim_bl; ++z) {
    for (size_t x = 0; x < yockcraft::world_dim_bl; ++x) {
      size_t img_idx = 4 * (x + z * yockcraft::world_dim_bl);

      std::array<uint8_t, 4> col = {0x00, 0x00, 0x00, 0xFF};
      float n = noise_data->noise[x + z * yockcraft::world_dim_bl];

      if (type == yockcraft::HeightMapType::NOISE) {
        col[yockcraft::ColorIndex::R] = col[yockcraft::ColorIndex::G] = col[yockcraft::ColorIndex::B] = (n >= 1.f) ? 
          255 : n * 256.f;
      } else {
        bool col_selected = false;
        for (int32_t i = 0; i < noise_data->terrains.size(); ++i) {
          if (n <= noise_data->terrains[i].height_max) {
            col[yockcraft::ColorIndex::R] = (uint8_t)(noise_data->terrains[i].rgb[yockcraft::ColorIndex::R] * 0xFF);
            col[yockcraft::ColorIndex::G] = (uint8_t)(noise_data->terrains[i].rgb[yockcraft::ColorIndex::G] * 0xFF);
            col[yockcraft::ColorIndex::B] = (uint8_t)(noise_data->terrains[i].rgb[yockcraft::ColorIndex::B] * 0xFF);
            col_selected = true;
            break;
          }
        }

        if (!col_selected) {
          col[yockcraft::ColorIndex::R] = col[yockcraft::ColorIndex::G] = col[yockcraft::ColorIndex::B] = (n >= 1.f) ? 
            255 : n * 256.f;
        }
      }
      
      if (x % yockcraft::chunk_dim == 0 || z % yockcraft::chunk_dim == 0) {
        col[yockcraft::ColorIndex::A] = 0x00;
      }
  
      noise_data->terrain_pixels[img_idx + yockcraft::ColorIndex::R] = col[yockcraft::ColorIndex::R];
      noise_data->terrain_pixels[img_idx + yockcraft::ColorIndex::G] = col[yockcraft::ColorIndex::G];
      noise_data->terrain_pixels[img_idx + yockcraft::ColorIndex::B] = col[yockcraft::ColorIndex::B];
      noise_data->terrain_pixels[img_idx + yockcraft::ColorIndex::A] = col[yockcraft::ColorIndex::A];
    }
  }
}

void GenerateTerrain(yockcraft::NoiseData* noise_data) {
  GenerateNoiseMap(noise_data);
  GenerateHeightMap(noise_data);
}

void DrawCameraControls(yockcraft::Camera& camera) {
  ImGui::Text("CAMERA COORD : [%f , %f , %f]", camera.Position().x , camera.Position().y, camera.Position().z);
}

void DrawTerrainTypeEditor(yockcraft::TerrainType& terr_type) {
  ImGui::Text("%s" , terr_type.name.c_str());
  ImGui::InputFloat("Height" , &terr_type.height_max , 0.f, 1.f);
  ImGui::ColorEdit3("RGB" , terr_type.rgb);
}

void CameraKeyboardCallback(yockcraft::Camera* camera) {
  float camera_speed = camera->Speed();
  glm::vec3 camera_position = camera->Position();

  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W]) {
    camera_position += camera_speed * camera->Front();
  }
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S]) {
    camera_position -= camera_speed * camera->Front();
  }
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A]) {
    camera_position -=
        glm::normalize(glm::cross(camera->Front(), camera->Up())) *
        camera_speed;
  }
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D]) {
    camera_position +=
        glm::normalize(glm::cross(camera->Front(), camera->Up())) *
        camera_speed;
  }
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]) {
    camera_position += camera_speed * camera->WorldUp();
  }
  if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LSHIFT]) {
    camera_position -= camera_speed * camera->WorldUp();
  }

  camera->SetPosition(camera_position);
}

void CameraMouseCallback(yockcraft::Camera* camera) {
  SDL_WarpMouseInWindow(nullptr, 1920 / 2, 1080 / 2);

  int x, y;
  SDL_GetGlobalMouseState(&x, &y);

  camera->SetLastMousePos(camera->MousePos());
  camera->SetMousePos({x, y});
  camera->SetMouseDelta({camera->MousePos().x - camera->LastMousePos().x,
                         camera->LastMousePos().y - camera->MousePos().y});

  int rel_x, rel_y;
  SDL_GetRelativeMouseState(&rel_x, &rel_y);

  camera->SetYaw(camera->Yaw() + (rel_x * camera->Sensitivity()));
  camera->SetPitch(camera->Pitch() - (rel_y * camera->Sensitivity()));

  if (camera->ConstrainPitch()) {
    if (camera->Pitch() > 89.0f) {
      camera->SetPitch(89.0f);
    }

    if (camera->Pitch() < -89.0f) {
      camera->SetPitch(-89.0f);
    }
  }

  glm::vec3 front;
  front.x =
      cos(glm::radians(camera->Yaw())) * cos(glm::radians(camera->Pitch()));
  front.y = sin(glm::radians(camera->Pitch()));
  front.z =
      sin(glm::radians(camera->Yaw())) * cos(glm::radians(camera->Pitch()));
  camera->SetFront(glm::normalize(front));
  camera->SetRight(
      glm::normalize(glm::cross(camera->Front(), camera->WorldUp())));
  camera->SetUp(glm::normalize(glm::cross(camera->Right(), camera->Front())));
  camera->SetPosition(camera->Position());
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  return main(__argc, __argv);
}
