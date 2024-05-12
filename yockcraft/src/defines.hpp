#ifndef YOCKCRAFT_DEFINES_HPP
#define YOCKCRAFT_DEFINES_HPP

#include <memory>
#include <type_traits>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <glm/glm.hpp>

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T , typename... Args>
Scope<T> NewScope(Args&&... args) {
  static_assert(std::is_constructible_v<T , Args...> , "Scope not constructible from given args!");
  return std::make_unique<T>(std::forward<Args>(args)...);
}

namespace yockcraft {

  /// chunk dimensions
  constexpr static const uint32_t chunk_dim = 16;
  constexpr static const int32_t schunk_dim = static_cast<int32_t>(chunk_dim);
  constexpr static const uint32_t chunk_height = 255;
  constexpr static const int32_t schunk_height = static_cast<int32_t>(chunk_height); 
  constexpr static const uint32_t chunk_area = chunk_dim * chunk_dim;
  constexpr static const int32_t schunk_area = static_cast<int32_t>(chunk_area);
  constexpr static const uint32_t chunk_volume = chunk_area * chunk_height;
  constexpr static const int32_t schunk_volume = static_cast<int32_t>(chunk_volume);
  constexpr static const uint32_t chunk_half_extents = chunk_dim / 2;
  constexpr static const int32_t schunk_half_extents = static_cast<int32_t>(chunk_half_extents); 

  /// world dimensions in chunks
  constexpr static const uint32_t world_dim = 16;
  constexpr static const int32_t sworld_dim = static_cast<int32_t>(world_dim);
  constexpr static const uint32_t world_height = 1;
  constexpr static const int32_t sworld_height = static_cast<int32_t>(world_height);
  constexpr static const uint32_t world_area = world_dim * world_dim;
  constexpr static const int32_t sworld_area = static_cast<int32_t>(world_area);
  constexpr static const uint32_t world_half_extents = world_dim / 2;
  constexpr static const int32_t sworld_half_extents = static_cast<int32_t>(world_half_extents);
  constexpr static const uint32_t world_volume  = world_dim * world_dim; /// height is 1
  constexpr static const int32_t sworld_volume  = static_cast<int32_t>(world_volume); /// height is 1

  /// world dimensions in blocks
  constexpr static const uint32_t world_dim_bl = world_dim * chunk_dim;  
  constexpr static const int32_t sworld_dim_bl = static_cast<int32_t>(world_dim_bl);  
  constexpr static const uint32_t world_half_extents_bl = world_dim_bl / 2;
  constexpr static const int32_t sworld_half_extents_bl = static_cast<int32_t>(world_half_extents_bl);
  constexpr static const uint32_t half_world_dim_bl = world_dim_bl / 2;  
  constexpr static const int32_t shalf_world_dim_bl = static_cast<int32_t>(half_world_dim_bl);  
  constexpr static const uint32_t world_height_bl = chunk_height; 
  constexpr static const int32_t sworld_height_bl = static_cast<int32_t>(world_height_bl); 
  constexpr static const uint32_t world_area_bl = world_dim_bl * world_dim_bl;
  constexpr static const int32_t sworld_area_bl = static_cast<int32_t>(world_area_bl);
  constexpr static const uint32_t world_volume_bl = world_volume * chunk_volume;
  constexpr static const int32_t sworld_volume_bl = static_cast<int32_t>(world_volume_bl);

  /// important vectors
  constexpr static const glm::ivec3 world_xz_offsets{
    world_half_extents , 0 , world_half_extents
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_DEFINES_HPP
