#ifndef YOCKCRAFT_VERTEX_HPP
#define YOCKCRAFT_VERTEX_HPP

#include <cstdint>
#include <vector>

#include <glad/glad.h>

#include "defines.hpp"

namespace yockcraft {

    enum BufferUsage {
        STATIC_DRAW = GL_STATIC_DRAW ,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW ,
        STREAM_DRAW = GL_STREAM_DRAW ,
    };
    
    enum BufferType {
        ARRAY_BUFFER = GL_ARRAY_BUFFER ,
        ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER ,
    };

    enum DrawMode {
        TRIANGLES = GL_TRIANGLES ,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP ,
        TRIANGLE_FAN = GL_TRIANGLE_FAN ,
        LINES = GL_LINES ,
        LINE_STRIP = GL_LINE_STRIP ,
        LINE_LOOP = GL_LINE_LOOP ,
        POINTS = GL_POINTS ,
    };

    class VertexBuffer {
        BufferType buffer_type;
        BufferUsage buffer_usage; 
        
        uint32_t renderer_id;
        uint32_t buffer_size;
        const void* buffer_data;
    
        public:
            VertexBuffer(
                const void* data , uint32_t size , 
                BufferUsage usage = STATIC_DRAW , 
                BufferType type = ARRAY_BUFFER
            );
            ~VertexBuffer();
    
            void Bind() const;
            void Unbind() const;
    };
    
    class VertexArray { 
        uint32_t renderer_id;
        uint32_t vertex_count;
    
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::vector<uint32_t> layout;
    
        Scope<VertexBuffer> vertex_buffer = nullptr;
        Scope<VertexBuffer> index_buffer = nullptr;
    
        void SetLayout();
    
        public:
            VertexArray(const std::vector<float>& vertices , const std::vector<uint32_t>& indices = {} , const std::vector<uint32_t>& layout = {}) ;
    
            ~VertexArray();
    
            void Bind() const;
            void Draw(DrawMode mode) const;
            void Unbind() const;
    };

} // namespace yockcraft

#endif // !YOCKCRAFT_VERTEX_HPP
