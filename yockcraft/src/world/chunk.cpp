#include "world/chunk.hpp"

#include <iostream>

#include <spdlog/fmt/fmt.h>
#include <glm/gtc/matrix_transform.hpp>

#include "defines.hpp"
#include "rendering/shader.hpp"

namespace yockcraft {

  TexUV GrassBlockUV(BlockFace face) {
    switch (face) {
      case BlockFace::SIDE:
        return TexUV{
          .min = { 3.0f / 32 , 15.0f / 16 } ,
          .max = { 4.0f / 32 , 16.0f / 16 } ,
        };
      case BlockFace::TOP:
        return TexUV{
          .min = { 2.0f / 32 , 15.0f / 16 } ,
          .max = { 3.0f / 32 , 16.0f / 16 } ,
        };
      case BlockFace::BOTTOM:
        return TexUV{
          .min = { 11.0f / 32 , 14.0f / 16 } , 
          .max = { 12.0f / 32 , 15.0f / 16 }
        };
      default:
        return TexUV{};
    }
  }
  
  TexUV StoneBlockUV() {
    return TexUV{
      .min = { 19.f / 32 , 15.f / 16 } ,
      .max = { 20.f / 32 , 16.f / 16.f }
    };
  }
  
  TexUV DirtBlockUV() {
    return TexUV{
      .min = { 11.0f / 32 , 14.0f / 16 } , 
      .max = { 12.0f / 32 , 15.0f / 16 }
    };
  }
  
  TexUV GetBlockUV(BlockType type, BlockFace face) {
    switch (type) {
      case BlockType::GRASS:
        return GrassBlockUV(face);
      case BlockType::DIRT:
        return DirtBlockUV();
      case BlockType::STONE:
        return StoneBlockUV();
      case BlockType::SAND:
      case BlockType::WATER:
      case BlockType::WOOD:
      case BlockType::LEAVES:
      case BlockType::CLOUD:
      case BlockType::SNOW:
      case BlockType::ICE:
      case BlockType::AIR:
      default:
        throw std::runtime_error("Why we here");
    }
  }
  
  Chunk::Chunk(glm::ivec3 world_position, const Terrain& terrain)
      : terrain(terrain), world_position(world_position) {
    zeroed_position = {
      world_position.x + (float)yockcraft::half_world_dim_bl ,
      world_position.y ,
      world_position.z + (float)yockcraft::half_world_dim_bl
    };
    // GenerateTerrain();
    GenerateMesh();
  }
  
  Chunk::~Chunk() {}
  
  void Chunk::Draw(Shader* shader) const {
    shader->SetUniform("model" , glm::mat4(1.0));
    cube->Draw(DrawMode::TRIANGLES);
  }
  
  glm::vec3 Chunk::ChunkDimensions() {
    return glm::vec3(static_cast<float>(chunk_dim));
  }
  
  bool Chunk::HasBlockAt(const glm::ivec3& pos) const {
    return pos.y <= terrain.blocks[(pos.x + zeroed_position.x) + (pos.z + zeroed_position.z) * world_dim_bl];
  }
  
  BlockType Chunk::GetBlockType(const glm::ivec3& pos) const {
    if (pos.y < yockcraft::chunk_height / 3) {
      return yockcraft::BlockType::STONE;
    } else if (pos.y >= yockcraft::chunk_height / 3 && pos.y < yockcraft::chunk_height - 1) {
      return yockcraft::BlockType::DIRT;
    } else {
      return yockcraft::BlockType::GRASS;
    }
  }
  
  void Chunk::AddVertex(const glm::vec3& vert , const glm::vec3& normal , const glm::vec2& uvs) {
    glm::vec3 rvert = vert;
  
    /// shift vertices on the edge of the chunk inwards a miniscule amount to avoid z-fighting
    if ((int32_t)rvert.x == 0) {
      rvert.x += 0.01f;
    } else if ((int32_t)rvert.x == chunk_dim) {
      rvert.x -= 0.01f;
    }
  
    if ((int32_t)rvert.y == 0) {
      rvert.y += 0.01f;
    } else if ((int32_t)rvert.y == chunk_height) {
      rvert.y -= 0.01f;
    }
  
    if ((int32_t)rvert.z == 0) {
      rvert.z += 0.01f;
    } else if ((int32_t)rvert.z == chunk_dim) {
      rvert.z -= 0.01f;
    }
  
    vertex_arr.push_back(rvert.x + world_position.x);
    vertex_arr.push_back(rvert.y + world_position.y);
    vertex_arr.push_back(rvert.z + world_position.z);
    vertex_arr.push_back(normal.x);
    vertex_arr.push_back(normal.y);
    vertex_arr.push_back(normal.z);
    vertex_arr.push_back(uvs.x);
    vertex_arr.push_back(uvs.y);
  }
  
  void Chunk::AddIndices(int32_t i0, int32_t i1, int32_t i2) {
    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);
  }
  
  void Chunk::AddQuad(const glm::ivec3& v0 , const glm::ivec3& v1 ,
                      const glm::ivec3& v2 , const glm::ivec3& v3 , 
                      const glm::ivec3& normal , const TexUV& uvs) {
    AddVertex(v0, normal , uvs.max);
    AddVertex(v1, normal , { uvs.min.x , uvs.max.y });
    AddVertex(v2, normal , uvs.min);
    AddVertex(v3, normal , { uvs.max.x , uvs.min.y });
  
    AddIndices(index , index + 1 , index + 2);
    AddIndices(index , index + 2 , index + 3);
    index += 4;
    ++total_quads;
  }
  
  void Chunk::XSweep() {
    int32_t n = 0;
  
    glm::ivec3 bl = { 0 , 0 , 0 };
    glm::ivec3 wh = { 1 , 1 , 0 };
  
    const size_t mask_size = chunk_dim * chunk_height;
  
    for (bl.z = -1; bl.z < schunk_dim;) {
      std::array<bool , mask_size> mask = {false};
      std::array<int8_t , mask_size> dir_mask = { 0 };
  
      for (int32_t y = 0; y < schunk_height; ++y) {
        for (int32_t x = 0; x < schunk_dim; ++x) {
          bool curr_exists  =  0 <= bl.z ? 
            !HasBlockAt({ x , y , bl.z }) : true;
          bool next_exists = bl.z + 1 < schunk_dim ? 
            !HasBlockAt({ x , y , bl.z + 1 }) : true;
  
          int64_t idx = x + y * schunk_dim;
          mask[idx] = curr_exists != next_exists; 

          if (mask[idx]) {
            if (curr_exists) {
              dir_mask[idx] = -1; 
            } else {
              dir_mask[idx] = 1;
            }
          }
        }
      }
  
      ++bl.z;
      n = 0;
  
      for (int32_t j = 0; j < schunk_height; ++j) {
        for (int32_t i = 0; i < schunk_dim;) {
          if (mask[n]) {
            bl.x = i;
            bl.y = j;

            glm::ivec3 normal = { 0 , 0 , dir_mask[n] };
            BlockFace face = BlockFace::SIDE;
            
            TexUV uvs = GetBlockUV(GetBlockType(bl) , face);

            if (normal.z == -1) {
              AddQuad({ bl.x , bl.y + wh.y , bl.z } ,
                      { bl.x + wh.x , bl.y + wh.y , bl.z } , 
                      { bl. x + wh.x , bl.y , bl.z } , 
                      { bl.x , bl.y , bl.z } ,
                      normal , uvs);
            } else {
              AddQuad({ bl.x + wh.x , bl.y + wh.y , bl.z } , 
                      { bl.x , bl.y + wh.y , bl.z } ,
                      { bl.x , bl.y , bl.z } ,
                      { bl. x + wh.x , bl.y , bl.z } , 
                      normal , uvs);
            }
  
            mask[n] = false;
          } 

          ++i;
          ++n;
        }
      }
    }
  }
  
  void Chunk::YSweep() {
    int32_t n = 0;
  
    glm::ivec3 bl = { 0 , 0 , 0 };
    glm::ivec3 wh = { 1 , 0 , 1 };
  
    const size_t mask_size = schunk_dim * schunk_dim;
  
    for (bl.y = -1; bl.y < schunk_height;) {
      std::array<bool, mask_size> mask = {false};
      std::array<int8_t , mask_size> dir_mask = { 0 };
      
      for (int32_t x = 0; x < schunk_dim; ++x) {
        for (int32_t z = 0; z < schunk_dim; ++z) {
          bool curr_exists  =  0 <= bl.y ? 
            !HasBlockAt({ x , bl.y , z }) : true;
          bool next_exists = bl.y + 1 < schunk_height ? 
            !HasBlockAt({ x , bl.y + 1 , z }) : true;
  
          int64_t idx = z + x * schunk_dim;
          mask[idx] = curr_exists != next_exists; 
          if (mask[idx]) {
            if (curr_exists) {
              dir_mask[idx] = -1;
            } else {
              dir_mask[idx] = 1;
            } 
          }
        }
      }
  
      ++bl.y;
      n = 0;
  
      for (int32_t j = 0; j < schunk_dim; ++j) {
        for (int32_t i = 0; i < schunk_dim;) {
          if (mask[n]) {
            bl.z = i;
            bl.x = j;
            
            /// this trick for the normals only works for top and bottom because 
            ///   the winding order is consistent in this setup, in the x,z sweeps 
            ///   the normal depends on which side the face is exposed on
            glm::ivec3 normal{ 0 , dir_mask[n] , 0 };
            BlockFace face = BlockFace::TOP;
            if (normal.y == -1) {
              face = BlockFace::BOTTOM;
            } 
            
            TexUV uvs = GetBlockUV(GetBlockType(bl) , face);

            if (normal.y == -1) {
              AddQuad({ bl.x + wh.x , bl.y , bl.z + wh.z } , 
                      { bl.x , bl.y , bl.z + wh.z } ,
                      { bl.x , bl.y , bl.z } ,
                      { bl.x + wh.x , bl.y , bl.z } ,
                      normal , uvs);
            } else {
              AddQuad({ bl.x + wh.x , bl.y , bl.z } ,
                      { bl.x , bl.y , bl.z } ,
                      { bl.x , bl.y , bl.z + wh.z } ,
                      { bl.x + wh.x , bl.y , bl.z + wh.z } , 
                      normal , uvs);
            }
  

            mask[n] = false;
          }

          ++i;
          ++n;
        }
      }
    }
  }
  
  void Chunk::ZSweep() {
    int32_t n = 0;
  
    glm::ivec3 bl = { 0 , 0 , 0 };
    glm::ivec3 wh = { 0 , 1 , 1 };
    
    const size_t mask_size = chunk_dim * chunk_height;
  
    for (bl.x = -1; bl.x < schunk_dim;) {
      std::array<bool, mask_size> mask = {false};
      std::array<int8_t , mask_size> dir_mask = { 0 };
      
      for (int32_t y = 0; y < schunk_height; ++y) {
        for (int32_t z = 0; z < schunk_dim; ++z) {
          bool curr_exists  =  0 <= bl.x ? 
            !HasBlockAt({ bl.x , y , z }) : true;
          bool next_exists = bl.x + 1 < schunk_dim ? 
            !HasBlockAt({ bl.x + 1 , y , z }) : true;
  
          int64_t idx = z + y * schunk_dim;
          mask[idx] = curr_exists != next_exists; 
          if (mask[idx]) {
            if (curr_exists) {
              dir_mask[idx] = -1;
            } else {
              dir_mask[idx] = 1;
            } 
          }
        }
      }
  
      ++bl.x;
      n = 0;
  
      for (int32_t j = 0; j < schunk_height; ++j) {
        for (int32_t i = 0; i < schunk_dim;) {
          if (mask[n]) {
            bl.z = i;
            bl.y = j;

            glm::ivec3 normal{ dir_mask[n] , 0 , 0 };
            BlockFace face = BlockFace::SIDE;

            TexUV uvs = GetBlockUV(GetBlockType(bl) , face);

            if (normal.x == -1) {
              AddQuad({ bl.x , bl.y + wh.y , bl.z + wh.z } , 
                      { bl.x , bl.y + wh.y , bl.z } ,
                      { bl.x , bl.y , bl.z } ,
                      { bl.x , bl.y , bl.z + wh.z } ,
                      normal, uvs);
            } else {
               AddQuad({ bl.x , bl.y + wh.y , bl.z } ,
                       { bl.x , bl.y + wh.y , bl.z + wh.z } , 
                       { bl.x , bl.y , bl.z + wh.z } ,
                       { bl.x , bl.y , bl.z } ,
                       normal, uvs);
            }
             
            mask[n] = false;
          } 

          ++i;
          ++n;
        }
      }
    }
  }
  
  Chunk::Mesh Chunk::GreedyMesh() {
    XSweep();
    YSweep();
    ZSweep();
    return { vertex_arr , indices };
  }

  void Chunk::GenerateTerrain() {
    for (size_t idx = 0; idx < yockcraft::chunk_area; ++idx) {
      for (size_t h = 0; h < yockcraft::chunk_height; ++h) {
        if (h < yockcraft::chunk_height / 3) {
          block_types[idx + h * yockcraft::chunk_area] = yockcraft::BlockType::STONE;
        } else if (h >= yockcraft::chunk_height / 3 || h < yockcraft::chunk_height - 1) {
          block_types[idx + h * yockcraft::chunk_area] = yockcraft::BlockType::DIRT;
        } else {
          block_types[idx + h * yockcraft::chunk_area] = yockcraft::BlockType::GRASS;
        }
      }
    }
  }
  
  void Chunk::GenerateMesh() {
    mesh = GreedyMesh();
    cube = NewScope<VertexArray>(mesh.vertices, mesh.indices,
                                 std::vector<uint32_t>{ 3  , 3  , 2 });
  }

}  // namespace yockcraft
