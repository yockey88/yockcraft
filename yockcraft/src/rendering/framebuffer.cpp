/**
 * \file rendering/framebuffer.cpp
 **/
#include "rendering/framebuffer.hpp"

#include <glad/glad.h>

#include "utils.hpp"
#include "defines.hpp"

namespace yockcraft {

  Framebuffer::Framebuffer(const glm::vec2& size , const glm::vec4& cc) 
      : size(size) , clear_color(cc) {
    glGenFramebuffers(1 , &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER , fbo);

    glGenTextures(1 , &color_attachment);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE , color_attachment);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE , 4 , GL_RGB , size.x , size.y , GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE , 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D_MULTISAMPLE , color_attachment , 0);

    glGenRenderbuffers(1 , &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER , rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER , 4, GL_DEPTH24_STENCIL8 , size.x , size.y);
    glBindRenderbuffer(GL_RENDERBUFFER , 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_STENCIL_ATTACHMENT , GL_RENDERBUFFER , rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      utils::printerr("Framebuffer is not complete!");
      glDeleteTextures(1 , &color_attachment);
      glDeleteRenderbuffers(1 , &rbo);
      glDeleteFramebuffers(1 , &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER , 0);
      return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER , 0);

    glGenFramebuffers(1 , &intermediate_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER , intermediate_fbo);

    glGenTextures(1 , &screen_texture);
    glBindTexture(GL_TEXTURE_2D , screen_texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      utils::printerr("Framebuffer is not complete!");
      glDeleteTextures(1 , &color_attachment);
      glDeleteRenderbuffers(1 , &rbo);
      glDeleteFramebuffers(1 , &fbo);
      glDeleteFramebuffers(1 , &intermediate_fbo);
      glDeleteTextures(1 , &screen_texture);
      glBindFramebuffer(GL_FRAMEBUFFER , 0);
      return;
    } 

    fb_complete = true;

    vao = NewScope<VertexArray>(
      std::vector<float>{
         1.f ,  1.f , 1.f , 1.f ,
        -1.f ,  1.f , 0.f , 1.f ,
        -1.f , -1.f , 0.f , 0.f ,
         1.f , -1.f , 1.f , 0.f
      } ,
      std::vector<uint32_t>{ 0 , 1 , 3 , 1 , 2 , 3 } ,
      std::vector<uint32_t>{ 2 , 2 }
    );

    glBindFramebuffer(GL_FRAMEBUFFER , 0);
  }
  
  Framebuffer::~Framebuffer() {
    glDeleteTextures(1 , &color_attachment);
    glDeleteRenderbuffers(1 , &rbo);
    glDeleteFramebuffers(1 , &fbo);
    glDeleteFramebuffers(1 , &intermediate_fbo);
    glDeleteTextures(1 , &screen_texture);
  }

  uint32_t Framebuffer::Texture() const {
    return screen_texture;
  }

  bool Framebuffer::Valid() const {
    return fb_complete;
  }

  bool Framebuffer::IsBound() const {
    return bound;
  }
      
  void Framebuffer::AttachShader(const std::string& vert_path ,
                    const std::string& frag_path ,
                    std::optional<std::string> geom_path) {
    if (!fb_complete) {
      return;
    }

    if (geom_path.has_value()) {
      shader = NewScope<Shader>(vert_path , frag_path , geom_path.value());
    } else {
      shader = NewScope<Shader>(vert_path , frag_path);
    }
    
    if (shader == nullptr || !shader->IsValid()) {
      // in case it was not null but invalid
      shader = nullptr;
      return;
    }

    shader->Bind();
    shader->SetUniform("fb_tex" , 0);
    shader->Unbind();
  }

  void Framebuffer::DetachShader() {
    shader = nullptr;
  }

  void Framebuffer::BindFrame() {
    if (!fb_complete) {
      return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER , fbo);
    glEnable(GL_DEPTH_TEST);

    bound = true;
  }

  void Framebuffer::UnbindFrame() {
    if (!fb_complete) {
      return;
    }

    int32_t clear_flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
    glBindFramebuffer(GL_READ_FRAMEBUFFER , fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER , intermediate_fbo);
    glBlitFramebuffer(0 , 0 , size.x , size.y , 0 , 0 , size.x , size.y , clear_flags , GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER , 0);
    glDisable(GL_DEPTH_TEST);

    bound = false;
  }

  void Framebuffer::Draw(DrawMode mode) {
    if (failed_render) {
      return;
    }

    if (!fb_complete || vao == nullptr || shader == nullptr) {
      utils::printerr("Rendering Invalid Framebuffer");
      failed_render = true;
      return;
    } else {
      shader->Bind();

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D , screen_texture);
      
      vao->Draw(mode);

      glBindTexture(GL_TEXTURE_2D , 0);

      shader->Unbind();
    }
  }

} // namespace yockcraft 
