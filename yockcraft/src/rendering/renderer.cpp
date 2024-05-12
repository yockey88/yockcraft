/**
 * \file rendering/renderer.cp
 **/
#include "rendering/renderer.hpp"

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace yockcraft {

  RenderState Renderer::state;

  FrameData Renderer::frame_data;

  RenderState* Renderer::Initialize(const RendererSpecification& spec) {
    state.window = GetWindow();
    if (state.window == nullptr) {
      return nullptr;
    } 
    
    state.framebuffer = NewScope<yockcraft::Framebuffer>(state.window->size , state.window->clear_color);
    state.framebuffer->AttachShader("yockcraft/resources/shaders/fbshader.vert" , 
                                    "yockcraft/resources/shaders/fbshader.frag");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForOpenGL(state.window->window, state.window->context);
    ImGui_ImplOpenGL3_Init("#version 460");

    return &state;
  }

  void Renderer::BeginRender(Camera* camera) {
    if (camera != nullptr && camera != frame_data.camera) {
      frame_data.camera = camera;
    }

    glViewport(0 , 0 , static_cast<uint32_t>(state.window->size.x) , static_cast<uint32_t>(state.window->size.y));
    glClearColor(state.window->clear_color.r, state.window->clear_color.g,
                 state.window->clear_color.b, state.window->clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void Renderer::EndRender() {
    while (!frame_data.render_calls.empty()) {
      RenderCall& call = frame_data.render_calls.front();

      if (call.shader != nullptr) {
        call.shader->Bind();
        if (frame_data.camera != nullptr) {
          call.shader->SetUniform("projection" , frame_data.camera->Projection());
          call.shader->SetUniform("view" , frame_data.camera->View());
        }
        
        if (call.texture != nullptr) {
          call.texture->Bind();
        }

        if (call.mesh != nullptr) {
          call.mesh->Draw(call.draw_mode);
        } 

        if (call.callback != nullptr) {
          call.callback();
        }
        
        if (call.texture != nullptr) {
          call.texture->Unbind();
        }

        call.shader->Unbind();
        
        frame_data.render_calls.pop();
      }
    }
  }

  void Renderer::SwapBuffers() {
    SDL_GL_MakeCurrent(state.window->window , state.window->context);
    SDL_GL_SwapWindow(state.window->window);
  }

  void Renderer::BeginFrame(Camera* camera) {
    if (camera != nullptr && camera != frame_data.camera) {
      frame_data.camera = camera;
    }

    state.framebuffer->BindFrame();
  }

  void Renderer::EndFrame() {
    state.framebuffer->UnbindFrame(); 
    frame_data.camera = nullptr;

    glClearColor(1.f , 1.f , 1.f , 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
  }
      
  void Renderer::BeginUIRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(state.window->window);
    ImGui::NewFrame();
  }

  void Renderer::FinishUIRender() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  void Renderer::SubmitRenderCall(RenderCall call) {
    frame_data.render_calls.push(call); 
  }

  void Renderer::Shutdown() {
    ImGui::DestroyContext();
    DestroyWindow(state.window);
  }

} // namespace yockcraft
