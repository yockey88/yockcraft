#ifndef YOCKCRAFT_CHUNK_HPP
#define YOCKCRAFT_CHUNK_HPP

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "defines.hpp"
#include "rendering/vertex.hpp"
#include "world/terrain.hpp"

namespace yockcraft {

  class Shader;

  enum class BlockFace {
    SIDE ,
    TOP   ,
    BOTTOM
  };
  
  struct TexUV {
    glm::vec2 min{ 0.f , 0.f };
    glm::vec2 max{ 1.f , 1.f };
  };
  
  
  TexUV GrassBlockUV(BlockFace face);
  TexUV StoneBlockUV();
  TexUV DirtBlockUV();

  TexUV GetBlockUV(BlockType type , BlockFace face);
 
  class Chunk {
    public:
      Chunk(glm::ivec3 world_position , const Terrain& terrain);
      ~Chunk();

      void Draw(Shader* shader) const;

      inline const uint32_t TerrainTextureID() const { return terrain.tex_id; }
      static glm::vec3 ChunkDimensions();

    private:
      struct Mesh {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
      } mesh;
      Mesh outline_mesh;

      const Terrain& terrain;
      std::array<BlockType , yockcraft::chunk_volume> block_types = {};
      
      glm::vec3 world_position; 
      /// zeroed as in shifted up to center
      glm::vec3 zeroed_position;

      ///> formula for index := x + z * dim + y * height * dim (for y-up coordinates)
      ///   true if block is solid , false if block is air
      std::array<bool , chunk_volume> blocks = { false };

      int32_t index = 0;
      std::vector<float> vertex_arr = {};
      std::vector<uint32_t> indices = {};
      std::vector<glm::vec3> vertices = {};
      uint32_t total_quads = 0;

      Scope<VertexArray> cube = nullptr;

      bool HasBlockAt(const glm::ivec3& pos) const;
      BlockType GetBlockType(const glm::ivec3& pos) const;

      void AddVertex(const glm::vec3& vert , const glm::vec3& normal , const glm::vec2& uvs);
      void AddIndices(int32_t i0 , int32_t i1 , int32_t i2);
      void AddQuad(const glm::ivec3& v0 , const glm::ivec3& v1 ,
                   const glm::ivec3& v2 , const glm::ivec3& v3 , 
                   const glm::ivec3& normal , const TexUV& uvs);

      void XSweep();
      void YSweep();
      void ZSweep();
      Mesh GreedyMesh();

      void GenerateTerrain();
      void GenerateMesh();
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_CHUNK_HPP
