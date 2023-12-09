#include "world/chunk.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "rendering/shader.hpp"

namespace yockcraft {

    Chunk::Chunk() {
        static const std::vector<float> vertices = {
            ///> cube vertices
            // top
             0.5f, 0.5f, -0.5f ,
            -0.5f, 0.5f, -0.5f ,
            -0.5f, 0.5f,  0.5f ,
             0.5f, 0.5f,  0.5f ,
        
            // bottom
             0.5f, -0.5f, -0.5f ,
             0.5f, -0.5f,  0.5f ,
            -0.5f, -0.5f,  0.5f ,
            -0.5f, -0.5f, -0.5f ,
        
            // left
            -0.5f,  0.5f,  0.5f ,
            -0.5f,  0.5f, -0.5f ,
            -0.5f, -0.5f, -0.5f ,
            -0.5f, -0.5f,  0.5f ,
        
            // right
             0.5f,  0.5f, -0.5f ,
             0.5f,  0.5f,  0.5f ,
             0.5f, -0.5f,  0.5f ,
             0.5f, -0.5f, -0.5f ,
        
            // front
             0.5f,  0.5f, 0.5f ,
            -0.5f,  0.5f, 0.5f ,
            -0.5f, -0.5f, 0.5f ,
             0.5f, -0.5f, 0.5f ,
        
            // back
             0.5f,  0.5f, -0.5f ,
             0.5f, -0.5f, -0.5f ,
            -0.5f, -0.5f, -0.5f ,
            -0.5f,  0.5f, -0.5f ,
        };
        
        static const std::vector<uint32_t> indices = {
            0 , 1 , 2 ,
            0 , 2 , 3 ,
        
            4 , 5 , 6 ,
            4 , 6 , 7 ,
        
            8 , 9 , 10 ,
            8 , 10 , 11 ,
        
            12 , 13 , 14 ,
            12 , 14 , 15 ,
        
            16 , 17 , 18 ,
            16 , 18 , 19 ,
        
            20 , 21 , 22 ,
            20 , 22 , 23 ,
        };

        cube = std::make_unique<VertexArray>(vertices , indices);

        size_t i = 0;
        for (int32_t z = 0; z < chunk_dim; ++z) {
            for (int32_t y = 0; y < chunk_dim; ++y) {
                for (int32_t x = 0; x < chunk_dim; ++x) {
                    positions[i] = glm::vec3(
                        x - static_cast<float>(chunk_dim) / 2 , 
                        y - static_cast<float>(chunk_dim) / 2 , 
                        z - static_cast<float>(chunk_dim) / 2 
                    );
                    ++i;
                }
            }
        }

    }

    Chunk::~Chunk() {}

    void Chunk::Draw(Shader* shader) const {
        glm::mat4 model = glm::mat4(1.0f);

        for (const auto& p : positions) {
            model = glm::translate(glm::mat4(1.0f) , p);
            shader->SetUniform("model" , model);
            cube->Bind();
            cube->Draw(DrawMode::TRIANGLES);
        }
    }

    glm::vec3 Chunk::ChunkDimensions() {
        return glm::vec3(static_cast<float>(chunk_dim));
    }

} // namespace yockcraft
