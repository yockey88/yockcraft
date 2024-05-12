#include "utils.hpp"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

namespace yockcraft {
namespace utils {

  std::string ReadFile(const std::string& file) {
    std::ifstream stream(file);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
  }

} // namespace utils

  bool FloatEQ(float a , float b) {
    return glm::abs(a - b) <= eps;
  }

  float Clamp(float min , float max , float val) {
    if (val <= min) {
      return min;
    }

    return val > max ?
      max : val;
  }

  float Clamp01(float val) {
    return Clamp(0.f , 1.f , val);
  }
  
  float InverseLerp(float min , float max , float val) {
    if (min == max) {
      return 0.f;
    }

    return Clamp01((val - min) / (max - min));
  }

} // namespace yockcraft
