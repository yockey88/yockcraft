#include "yockcraft.hpp"

#include <defines.hpp>
#include <filesystem>

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "utils.hpp"
#include "rendering/framebuffer.hpp"
#include "rendering/renderer.hpp"
#include "world/chunk.hpp"

void CameraKeyboardCallback(yockcraft::Camera* camera);
void CameraMouseCallback(yockcraft::Camera* camera);

static const std::vector<float> xaxis = {
  -(float)yockcraft::half_world_dim_bl , 0.f , 0.f ,
   (float)yockcraft::half_world_dim_bl , 0.f , 0.f ,
};

static const std::vector<float> yaxis = {
  0.f , -(float)yockcraft::half_world_dim_bl , 0.f ,
  0.f ,  (float)yockcraft::half_world_dim_bl , 0.f ,
};

static const std::vector<float> zaxis = {
  0.f , 0.f , -(float)yockcraft::half_world_dim_bl ,
  0.f , 0.f ,  (float)yockcraft::half_world_dim_bl ,
};

static const std::vector<float> grid_rect_verts = {
   static_cast<float>(yockcraft::half_world_dim_bl) , 0.f , -static_cast<float>(yockcraft::half_world_dim_bl) ,
  -static_cast<float>(yockcraft::half_world_dim_bl) , 0.f , -static_cast<float>(yockcraft::half_world_dim_bl) ,
  -static_cast<float>(yockcraft::half_world_dim_bl) , 0.f ,  static_cast<float>(yockcraft::half_world_dim_bl) ,
   static_cast<float>(yockcraft::half_world_dim_bl) , 0.f ,  static_cast<float>(yockcraft::half_world_dim_bl) ,
};

static const std::string atlas_path = "yockcraft/resources/textures/blocks0.png";

void DrawLineMesh(yockcraft::Shader& shader , yockcraft::VertexArray& mesh , const glm::vec3& line_color) {
  shader.SetUniform("model", glm::mat4(1.0f));
  shader.SetUniform("uni_color", line_color);
  mesh.Draw(yockcraft::DrawMode::LINES);
}

void DrawFullGrid(yockcraft::Shader& shader , yockcraft::VertexArray& xaxis , yockcraft::VertexArray& zaxis);

int main(int argc , char* argv[]) {
  /// this is for debug reasons Visual Studio debugger opens it in different cwd
  /// and I'm too lazy to find out how to change working directory through premake generation
  if (argc > 1) {
    std::filesystem::current_path(std::filesystem::path("../"));
  }

  yockcraft::State state;

  /// control variables for the gui 
  float normal_mag = 1.5f;

  yockcraft::RenderState* renderer = yockcraft::Renderer::Initialize({});

  {
    /// camera initialization
    yockcraft::Camera camera;
    camera.SetPosition(glm::vec3(0.0f));
    camera.SetSpeed(0.5f);

    /// grid initialization
    yockcraft::VertexArray xaxis_vao(xaxis);
    yockcraft::VertexArray yaxis_vao(yaxis);
    yockcraft::VertexArray zaxis_vao(zaxis);

    yockcraft::Shader line_shader("yockcraft/resources/shaders/line.vert" , 
                                  "yockcraft/resources/shaders/line.frag");
    if (!line_shader.IsValid()) {
      std::cout << "Failed to create line shader" << std::endl;
    }

    /// test chunk initialization
    yockcraft::Terrain terrain{};
    for (size_t idx = 0; idx < yockcraft::world_dim_bl * yockcraft::world_dim_bl; ++idx) {
      terrain.blocks[idx] = yockcraft::chunk_height;
    }
    Scope<yockcraft::Chunk> chunk = NewScope<yockcraft::Chunk>(glm::vec3{ -yockcraft::schunk_dim / 2 , 0 , -yockcraft::schunk_dim / 2 } , terrain);
    yockcraft::Shader cshader("yockcraft/resources/shaders/shader.vert" , "yockcraft/resources/shaders/shader.frag");
    if (!cshader.IsValid()) {
      yockcraft::utils::printerr("failed to create chunk shader");
    }

    yockcraft::Shader cshader_normal("yockcraft/resources/shaders/normals.vert" , 
                                     "yockcraft/resources/shaders/normals.frag" ,
                                     "yockcraft/resources/shaders/normals.geom");
    if (!cshader_normal.IsValid()) {
      yockcraft::utils::printerr("failed to create normal visualizing shader");
    }

    /// block texture atlas
    yockcraft::Texture atlas(atlas_path);

    std::chrono::time_point<std::chrono::system_clock> now, last;
    while (state.running) {
      last = now;
      now = std::chrono::system_clock::now();
      std::chrono::duration<float> delta = now - last;

      PollEvents(renderer->window , state);
      if (state.camera_free) {
        CameraKeyboardCallback(&camera);
        CameraMouseCallback(&camera);
      }

      int32_t mouse_x , mouse_y;
      SDL_GetMouseState(&mouse_x , &mouse_y);

      glm::vec3 mouse_screen_pos{
        static_cast<float>(mouse_x) / renderer->window->size.x ,
        static_cast<float>(mouse_y) / renderer->window->size.y ,
        1.f
      };

      glm::vec4 homo_clip_pos{
        mouse_screen_pos.x , mouse_screen_pos.y ,
        -1.f , 1.f
      };

      glm::vec4 cam_pos = glm::inverse(camera.Projection()) * homo_clip_pos;
      glm::vec4 eye_pos{
        cam_pos.x , cam_pos.y ,
        -1.f , 1.f
      };

      glm::vec4 intermediate_world_pos = glm::inverse(camera.View()) * eye_pos;
      glm::vec3 world_pos = glm::normalize(glm::vec3{
        intermediate_world_pos.x , 
        intermediate_world_pos.y ,
        intermediate_world_pos.z 
      });

      yockcraft::Renderer::BeginFrame(&camera);
      yockcraft::Renderer::BeginRender();

      yockcraft::Renderer::SubmitRenderCall({
        yockcraft::DrawMode::TRIANGLES ,
        &line_shader ,
        nullptr , 
        nullptr ,
        std::bind([&]() {
          DrawLineMesh(line_shader , xaxis_vao , { 1.f , 0.f , 0.f });
          DrawLineMesh(line_shader , yaxis_vao , { 0.f , 1.f , 0.f });
          DrawLineMesh(line_shader , zaxis_vao , { 0.f , 0.f , 1.f });
          DrawFullGrid(line_shader , xaxis_vao , zaxis_vao);
        })
      });

      yockcraft::Renderer::SubmitRenderCall({
        yockcraft::DrawMode::TRIANGLES ,
        &cshader ,
        &atlas ,
        nullptr ,
        std::bind([&chunk , &cshader, &atlas]() -> void {
          cshader.SetUniform("atlas" , atlas.ID());
          chunk->Draw(&cshader); 
        })
      });

      yockcraft::Renderer::SubmitRenderCall({
        yockcraft::DrawMode::TRIANGLES ,
        &cshader_normal ,
        nullptr ,
        nullptr ,
        std::bind([&chunk , &cshader_normal, &normal_mag]() -> void {
          cshader_normal.SetUniform("magnitude" , normal_mag);
          chunk->Draw(&cshader_normal);
        })
      });

      glPolygonMode(GL_FRONT_AND_BACK, state.render_mode);
      yockcraft::Renderer::EndRender();
      glPolygonMode(GL_FRONT_AND_BACK , yockcraft::RenderMode::FILLED);
      yockcraft::Renderer::EndFrame();
      
      renderer->framebuffer->Draw(yockcraft::DrawMode::TRIANGLES);
      
#if 1
      yockcraft::Renderer::BeginUIRender();

      if (ImGui::Begin("Controls")) {
        ImGui::Text("FPS :: [%f]", 1.0f / delta.count());
        ImGui::Separator();

        ImGui::SliderFloat("Normals" , &normal_mag, 1.f, 10.f);
      }
      ImGui::End();

      yockcraft::Renderer::FinishUIRender();
#endif
      
      yockcraft::Renderer::SwapBuffers();
    }
  }

  yockcraft::Renderer::Shutdown();

  return 0;
}

void DrawFullGrid(yockcraft::Shader& shader , yockcraft::VertexArray& xaxis , yockcraft::VertexArray& zaxis) {
  /// other x-axes
  shader.SetUniform("uni_color", glm::vec3(0.37f));
  for (int z = -64; z <= 64; ++z) {
    if (z == 0) {
      continue;
    }
    shader.SetUniform("model" , glm::translate(glm::mat4(1.f), {0, 0, z}));
    xaxis.Draw(yockcraft::DrawMode::LINES);
  }

  /// other z-axes
  shader.SetUniform("uni_color", glm::vec3(0.37f));
  for (int x = -64; x <= 64; ++x) {
    if (x == 0) {
      continue;
    }
    shader.SetUniform("model" , glm::translate(glm::mat4(1.f), {x, 0, 0}));
    zaxis.Draw(yockcraft::DrawMode::LINES);
  }
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

int WINAPI WinMain(HINSTANCE inst , HINSTANCE prev_inst , LPSTR cmdline , int cmdshow) {
  return main(__argc , __argv);
}
