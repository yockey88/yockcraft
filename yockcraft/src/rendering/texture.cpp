#include "rendering/texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace yockcraft {

Texture::Texture(const std::string& file, TextureType type)
    : files({file}), type(type) {
  glGenTextures(1, &renderer_id);

  glBindTexture(type, renderer_id);
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int32_t channels = 0;

  stbi_set_flip_vertically_on_load(true);
  uint8_t* data = stbi_load(file.c_str(), &size.x, &size.y, &channels, 0);
  if (data == nullptr) {
    std::cout << "Failed to load texture" << std::endl;
    return;
  }

  switch (channels) {
    case 1:
      format = RED;
      break;
    case 2:
      format = RG;
      break;
    case 3:
      format = RGB;
      break;
    case 4:
      format = RGBA;
      break;
    default:
      break;
  }

  glTexImage2D(type, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE,
               data);

  glGenerateMipmap(type);

  stbi_image_free(data);

  glBindTexture(type, 0);
}

Texture::Texture(const std::vector<std::string>& files)
    : files(files), type(TEXTURE_CUBE_MAP) {
  glGenTextures(1, &renderer_id);

  glBindTexture(type, renderer_id);
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  int32_t channels = 0;

  uint8_t* data = nullptr;
  for (uint32_t i = 0; i < files.size(); ++i) {
    data = stbi_load(files[i].c_str(), &size.x, &size.y, &channels, 0);
    if (data == nullptr) {
      std::cout << "Failed to load texture" << std::endl;
      return;
    }

    GLenum format = GL_RGB;
    switch (channels) {
      case 1:
        format = GL_RED;
        break;
      case 2:
        format = GL_RG;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        break;
    }

    // ideally be able to swap this function out for a more general one based on
    // the typ
    //   i.e glTexImage2D/glTexImage3D/glTexImageCube
    //   but without a function pointer parameter
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, size.x, size.y,
                 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
}

Texture::Texture(const void* data, glm::ivec2 size,
                 /* DataType = UINT8 */
                 TextureType type, TextureFormat format)
    : size(size), type(type), format(format) {
  glGenTextures(1, &renderer_id);

  glBindTexture(type, renderer_id);
  glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(type, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE,
               data);

  glGenerateMipmap(type);

  glBindTexture(type, 0);
}

Texture::~Texture() { glDeleteTextures(1, &renderer_id); }

void Texture::Bind(uint32_t pos) const {
  glActiveTexture(GL_TEXTURE0 + pos);
  glBindTexture(type, renderer_id);
}

void Texture::Unbind() const { glBindTexture(type, 0); }

}  // namespace yockcraft
