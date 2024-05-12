/**
 * \file world/terrain.cpp
 **/
#include "world/terrain.hpp"

#include <glm/glm.hpp>

namespace yockcraft {

  bool Terrain::HasBlockAt(const glm::vec3& pos) {
    uint32_t idx = pos.x + pos.z * world_dim;
    return pos.y <= blocks[idx];
  }

} // namespace yockcraft
