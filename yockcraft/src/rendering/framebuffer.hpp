/**
 * \file rendering/framebuffer.hpp
 **/
#ifndef YOCKCRAFT_FRAMEBUFFER_HPP
#define YOCKCRAFT_FRAMEBUFFER_HPP

#include <cstdint>
#include <optional>

#include <glm/glm.hpp>

#include "defines.hpp"
#include "rendering/shader.hpp"
#include "rendering/vertex.hpp"
#include "rendering/render_defines.hpp"

namespace yockcraft {

  class Framebuffer {
    public:
      Framebuffer(const glm::vec2& size , const glm::vec4& cc);
      ~Framebuffer();

      uint32_t Texture() const;
      bool Valid() const;
      bool IsBound() const;

      void AttachShader(const std::string& vert_path ,
                        const std::string& frag_path ,
                        std::optional<std::string> geom_path = std::nullopt);
      void DetachShader();

      void BindFrame();
      void UnbindFrame();

      void Draw(DrawMode mode);

    private:
      uint32_t fbo = 0;
      uint32_t rbo = 0;
      uint32_t framebuffer = 0;

      uint32_t intermediate_fbo = 0;
      uint32_t screen_texture = 0;

      uint32_t color_attachment = 0;

      glm::vec2 size = glm::vec2(0.0f);
      glm::vec4 clear_color = glm::vec4(0.0f);

      Scope<Shader> shader = nullptr;
      Scope<VertexArray> vao = nullptr;

      bool bound = false;
      bool fb_complete = false;
      bool failed_render = false;
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_FRAMEBUFFER_HPP
