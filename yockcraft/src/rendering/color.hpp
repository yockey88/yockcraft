/**
 * \file rendering/color.hpp
 **/
#ifndef YOCKCRAFT_COLOR_HPP
#define YOCKCRAFT_COLOR_HPP

#include <glm/glm.hpp>

namespace yockcraft {

  enum ColorIndex {
    R = 0 ,
    G ,
    B ,
    A
  };

  // class Color {
  //   public:
  //     Color() 
  //         : rgb_0255(0) , rgb_01(0.f) {}
  //     Color(uint8_t color)
  //         : rgb_0255(color) {
  //       rgb_01 = glm::vec4(static_cast<float>(color) / 255);
  //     }
  //     Color(uint8_t r , uint8_t g , uint8_t b) 
  //         : rgb_0255(r , g , b , 255) {
  //       rgb_01.r = static_cast<float>(r) / 255;
  //       rgb_01.g = static_cast<float>(g) / 255;
  //       rgb_01.b = static_cast<float>(b) / 255;
  //       rgb_01.a = 1.f;
  //     }

  //     void SetAlpha(uint8_t alpha) {
  //       rgb_0255.a

  //     const float R() const { return rgb_01.r; }     
  //     const float G() const { return rgb_01.g; }     
  //     const float B() const { return rgb_01.b; }     
  //     const glm::ivec4& GetRGBA() const { return rgb_0255; }
  //     const glm::vec4& NormalizedRGBA() const { return rgb_01; }

  //   private:
  //     glm::ivec4 rgba_0255{};
  //     glm::vec4 rgb_01{};
  // };

} // namespace yockcraft

#endif // !YOCKCRAFT_COLOR_HPP
