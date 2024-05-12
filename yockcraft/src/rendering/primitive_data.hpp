/**
 * \file rendering/primitive_data.hpp
 **/
#ifndef YOCKCRAFT_PRIMITIVE_DATA_HPP
#define YOCKCRAFT_PRIMITIVE_DATA_HPP

#include <vector>

#include "rendering/vertex.hpp"

namespace yockcraft {

  const static std::vector<float> kQuadVertices = {
    1.f , 1.f , 0.f ,
    0.f , 1.f , 0.f , 
    0.f , 0.f , 0.f ,
    1.f , 0.f , 0.f
  };

  const static std::vector<uint32_t> kIndices = {
    0 , 1 , 3 , 1 , 2 , 3
  };

  inline Scope<VertexArray> GetQuadMesh() {
    return NewScope<VertexArray>(kQuadVertices , kIndices);
  }

} // namespace yockcraft

#endif // !YOCKCRAFT_PRIMITIVE_DATA_HPP
