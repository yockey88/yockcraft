/**
 * \file rendering/renderer.hpp
*/
#ifndef YOCKCRAFT_RENDERER_HPP
#define YOCKCRAFT_RENDERER_HPP

#include <queue>
#include <functional>

#include <glad/glad.h>

#include "rendering/shader.hpp"
#include "rendering/vertex.hpp"
#include "rendering/texture.hpp"
#include "rendering/camera.hpp"
#include "rendering/framebuffer.hpp"
#include "rendering/window.hpp"

namespace yockcraft {
  
  enum RenderMode {
    POINT = GL_POINT ,
    WIREFRAME = GL_LINE , 
    FILLED = GL_FILL
  };

  struct RendererSpecification {
     
  };

  struct RenderState {
    Scope<Framebuffer> framebuffer = nullptr;
    Scope<Window> window = nullptr;
  };

  struct RenderCall {
    DrawMode draw_mode;
    Shader* shader;
    Texture* texture;
    VertexArray* mesh;
    std::function<void()> callback;
  };

  struct FrameData {
    Camera* camera = nullptr;
    std::queue<RenderCall> render_calls;
  };

  class Renderer {
    public:
      static RenderState* Initialize(const RendererSpecification& spec);

      static void BeginRender(Camera* camera = nullptr);
      static void EndRender();
      static void SwapBuffers();

      static void BeginFrame(Camera* camera = nullptr);
      static void EndFrame();

      static void BeginUIRender();
      static void FinishUIRender();

      static void SubmitRenderCall(RenderCall call);

      static void Shutdown();

    private:
      static RenderState state;

      static FrameData frame_data;
  };  

} // namespace yockcraft

#endif // !YOCKCRAFT_RENDERER_HPP
