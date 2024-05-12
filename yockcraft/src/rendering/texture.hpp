#ifndef YOCKCRAFT_TEXTURE_HPP
#define YOCKCRAFT_TEXTURE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace yockcraft {

    enum TextureType {
        TEXTURE_2D = GL_TEXTURE_2D ,
        TEXTURE_3D = GL_TEXTURE_3D ,
        TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP ,
    };
    
    enum TextureFormat {
        RED = GL_RED ,
        RG = GL_RG ,
        RGB = GL_RGB ,
        RGBA = GL_RGBA ,
    };

    class Texture {
        uint32_t renderer_id;
        std::vector<std::string> files;
    
        glm::ivec2 size;
    
        TextureType type;
        TextureFormat format;
    
        public:
            Texture(const std::string& file , TextureType type = TEXTURE_2D);
            Texture(const std::vector<std::string>& files);
            Texture(
                const void* data , glm::ivec2 size , 
                TextureType type = TEXTURE_2D , TextureFormat format = RGBA
            );
            ~Texture();
    
            void Bind(uint32_t pos = 0) const;
            void Unbind() const;

            inline uint32_t ID() const { return renderer_id; }
            inline const glm::ivec2& GetSize() const { return size; }
    };

} // namespace yockcraft

#endif // !YOCKCRAFT_TEXTURE_HPP
