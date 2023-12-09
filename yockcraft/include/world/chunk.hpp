#ifndef YOCKCRAFT_CHUNK_HPP
#define YOCKCRAFT_CHUNK_HPP

#include <array>

#include <glm/glm.hpp>

#include "defines.hpp"
#include "rendering/vertex.hpp"

namespace yockcraft {

    class Shader;

    class Chunk {
        constexpr static const uint32_t chunk_dim = 16;
        constexpr static const uint32_t chunk_volume = chunk_dim * chunk_dim * chunk_dim;
        Scope<VertexArray> cube = {};

        std::array<glm::vec3 , chunk_volume> positions = {};

        public:
            Chunk();
            ~Chunk();

            void Draw(Shader* shader) const;

            static glm::vec3 ChunkDimensions();
    };

} // namespace yockcraft

#endif // !YOCKCRAFT_CHUNK_HPP
