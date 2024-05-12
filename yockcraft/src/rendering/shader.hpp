#ifndef YOCKCRAFT_SHADER_HPP
#define YOCKCRAFT_SHADER_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <optional>

#include <glm/glm.hpp>

#include <glad/glad.h>

#include "rendering/render_defines.hpp"

namespace yockcraft {

  class Shader {
    public:
      Shader(const std::string& vertex_path , const std::string& fragment_path ,
             std::optional<std::string> geometry_path = std::nullopt);
      ~Shader();

      void Bind() const;
      void Unbind() const;

      inline const bool IsValid() const { return valid; }

      void SetUniform(const std::string& name , const uint32_t& value);
      void SetUniform(const std::string& name , const int32_t& value);
      void SetUniform(const std::string& name , const float& value);
      void SetUniform(const std::string& name , const glm::vec2& value);
      void SetUniform(const std::string& name , const glm::vec3& value);
      void SetUniform(const std::string& name , const glm::vec4& value);
      void SetUniform(const std::string& name , const glm::mat2& value);
      void SetUniform(const std::string& name , const glm::mat3& value);
      void SetUniform(const std::string& name , const glm::mat4& value);

    private:
      uint32_t renderer_id;

      std::string vertex_path;
      std::string fragment_path;
      std::optional<std::string> geometry_path;

      std::string vertex_src;
      std::string fragment_src;
      std::string geometry_src;

      std::unordered_map<std::string , int32_t> uniform_locations;

      bool valid = false;

      bool CompileShader(uint32_t shader_piece , const char* src);
      bool LinkShader();

      uint32_t GetUniformLocation(const std::string& name);
  };

} // namespace yockcraft

#endif // !YOCKCRAFT_SHADER_HPP
