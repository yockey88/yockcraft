#ifndef YOCKCRAFT_VERTEX_HPP
#define YOCKCRAFT_VERTEX_HPP

#include <cstdint>
#include <vector>

#include <glad/glad.h>

#include "defines.hpp"
#include "rendering/render_defines.hpp"

namespace yockcraft {

  class VertexBuffer { 
    public:
      VertexBuffer(
        const void* data , uint32_t size , 
        BufferUsage usage = STATIC_DRAW , 
        BufferType type = ARRAY_BUFFER
      );
      ~VertexBuffer();
  
      void Bind() const;
      void Unbind() const;

    private:
      BufferType buffer_type;
      BufferUsage buffer_usage; 
      
      uint32_t renderer_id;
      uint32_t buffer_size;
      const void* buffer_data;
  };
  
  class VertexArray { 
    public:
      VertexArray(const std::vector<float>& vertices , const std::vector<uint32_t>& indices = {} , const std::vector<uint32_t>& layout = {}) ;
  
      ~VertexArray();
  
      void Bind() const;
      void Draw(DrawMode mode) const;
      void Unbind() const;

    private:
      uint32_t renderer_id;
      uint32_t vertex_count;
  
      std::vector<float> vertices;
      std::vector<uint32_t> indices;
      std::vector<uint32_t> layout;
  
      Scope<VertexBuffer> vertex_buffer = nullptr;
      Scope<VertexBuffer> index_buffer = nullptr;
  
      void SetLayout();
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_VERTEX_HPP
