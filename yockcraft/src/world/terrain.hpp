/**
 * \file world/terrain.hpp
 **/
#ifndef YOCKCRAFT_TERRAIN_HPP
#define YOCKCRAFT_TERRAIN_HPP

#include <string>
#include <array>

#include "defines.hpp"

namespace yockcraft {
  
  enum class BlockType {
    AIR    ,
    GRASS  ,
    DIRT   ,
    STONE  ,
    SAND   ,
    WATER  ,
    WOOD   ,
    LEAVES ,
    CLOUD  ,
    SNOW   ,
    ICE    ,
    NUM_TYPES
  };

  struct TerrainType {
    int32_t id = 0;
    std::string name = "";
    float height_max = 0.f;
    float rgb[4] = { 0x00 , 0x00 , 0x00 , 0x00 };
  };

  struct Terrain {
    uint32_t tex_id = 0;
    std::array<uint8_t , world_area_bl> blocks = {}; 

    bool HasBlockAt(const glm::vec3& pos);
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_TERRAIN_HP
