#include "rendering/shader.hpp"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"

namespace yockcraft {

    bool Shader::CompileShader(uint32_t shader_piece , const char* src) {
        int32_t check = 0;
        glCompileShader(shader_piece);
        glGetShaderiv(shader_piece , GL_COMPILE_STATUS , &check);
        if (check != GL_TRUE) {
            int32_t length = 0;
            glGetShaderiv(shader_piece , GL_INFO_LOG_LENGTH , &length);
            std::vector<char> message(length);
            glGetShaderInfoLog(shader_piece , length , &length , message.data());
    
            std::string error(message.begin() , message.end());
            std::string type = shader_piece == VERTEX_SHADER ? "vertex" : "fragment";
            std::cout << "Failed to compile " << type << " shader: " << error << std::endl;
    
            return false;
        }
    
        return true;
    }
    
    bool Shader::LinkShader() {
        int32_t check = 0;
        glLinkProgram(renderer_id);
        glGetProgramiv(renderer_id , GL_LINK_STATUS , &check);
        if (check != GL_TRUE) {
            int32_t length = 0;
            glGetProgramiv(renderer_id , GL_INFO_LOG_LENGTH , &length);
            std::vector<char> message(length);
            glGetProgramInfoLog(renderer_id , length , &length , message.data());
    
            std::string error(message.begin() , message.end());
            std::cout << "Failed to link shader program: " << error << std::endl;
    
            return false;
        }
    
        return true;
    }
    
    Shader::Shader(const std::string& vertex_path , const std::string& fragment_path) 
            : vertex_path(vertex_path) , fragment_path(fragment_path) {
        vertex_src = utils::ReadFile(vertex_path);
        fragment_src = utils::ReadFile(fragment_path);
    
        const char* vsrc_ptr = vertex_src.c_str();
        const char* fsrc_ptr = fragment_src.c_str();
    
        uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    
        glShaderSource(vertex_shader , 1 , &vsrc_ptr , nullptr);
        glShaderSource(fragment_shader , 1 , &fsrc_ptr , nullptr);
    
        if (!CompileShader(vertex_shader , vsrc_ptr)) {
            return;
        }
    
        if (!CompileShader(fragment_shader , fsrc_ptr)) {
            return;
        }
    
        renderer_id = glCreateProgram();
        glAttachShader(renderer_id , vertex_shader);
        glAttachShader(renderer_id , fragment_shader);
    
        if (!LinkShader()) {
            return;
        }
    
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    
        valid = true;
    }
    
     Shader::~Shader() {
        glDeleteProgram(renderer_id);
     }
    
    void Shader::Bind() const {
        glUseProgram(renderer_id);
    }
    
    void Shader::Unbind() const {
        glUseProgram(0);
    }
    
    uint32_t Shader::GetUniformLocation(const std::string& name) {
        if (uniform_locations.find(name) != uniform_locations.end()) {
            return uniform_locations[name];
        }
    
        int32_t location = glGetUniformLocation(renderer_id , name.c_str());
        uniform_locations[name] = location;
    
        return location;
    }
    
    void Shader::SetUniform(const std::string& name , const int32_t& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniform1i(loc , value);
    }
    
    void Shader::SetUniform(const std::string& name , const float& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniform1f(loc , value);
    }
    
    void Shader::SetUniform(const std::string& name , const glm::vec2& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniform2f(loc , value.x , value.y);
    }
    
    void Shader::SetUniform(const std::string& name , const glm::vec3& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniform3f(loc , value.x , value.y , value.z);
    }
    
    void Shader::SetUniform(const std::string& name , const glm::vec4& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniform4f(loc , value.x , value.y , value.z , value.w);
    }
    
    void Shader::SetUniform(const std::string& name , const glm::mat2& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniformMatrix2fv(loc , 1 , GL_FALSE , glm::value_ptr(value));
    }
    
    void Shader::SetUniform(const std::string& name , const glm::mat3& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniformMatrix3fv(loc , 1 , GL_FALSE , glm::value_ptr(value));
    }
    
    void Shader::SetUniform(const std::string& name , const glm::mat4& value) {
        uint32_t loc = GetUniformLocation(name);
        glUniformMatrix4fv(loc , 1 , GL_FALSE , glm::value_ptr(value));
    }

} // namespace yockcraft
