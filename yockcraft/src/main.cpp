#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <SDL.h>
#undef main
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

///> Data

template <typename T>
using Scope = std::unique_ptr<T>;

static const std::vector<float> vertices = {
    ///> quad vertices
    //  1.0f ,  1.0f , 1.0f ,
    //  1.0f , -1.0f , 1.0f ,
    // -1.0f , -1.0f , 1.0f ,
    // -1.0f ,  1.0f , 1.0f ,
    
    ///> quad no indices
    //  1.0f ,  1.0f , 0.0f ,
    //  1.0f , -1.0f , 0.0f ,
    // -1.0f , -1.0f , 0.0f ,

    // -1.0f , -1.0f , 0.0f ,
    // -1.0f ,  1.0f , 0.0f ,
    //  1.0f ,  1.0f , 0.0f ,

    ///> cube vertices
    // top
     0.5f, 0.5f, -0.5f ,
    -0.5f, 0.5f, -0.5f ,
    -0.5f, 0.5f,  0.5f ,
     0.5f, 0.5f,  0.5f ,

    // bottom
     0.5f, -0.5f, -0.5f ,
    -0.5f, -0.5f, -0.5f ,
    -0.5f, -0.5f,  0.5f ,
     0.5f, -0.5f,  0.5f ,

    // left
    -0.5f,  0.5f,  0.5f ,
    -0.5f,  0.5f, -0.5f ,
    -0.5f, -0.5f, -0.5f ,
    -0.5f, -0.5f,  0.5f ,

    // right
     0.5f,  0.5f, -0.5f ,
     0.5f,  0.5f,  0.5f ,
     0.5f, -0.5f,  0.5f ,
     0.5f, -0.5f, -0.5f ,

    // front
     0.5f,  0.5f, 0.5f ,
    -0.5f,  0.5f, 0.5f ,
    -0.5f, -0.5f, 0.5f ,
     0.5f, -0.5f, 0.5f ,

    // back
     0.5f,  0.5f, -0.5f ,
    -0.5f,  0.5f, -0.5f ,
    -0.5f, -0.5f, -0.5f ,
     0.5f, -0.5f, -0.5f ,
};

static const std::vector<uint32_t> indices = {
    0 , 1 , 2 ,
    0 , 2 , 3 ,

    4 , 5 , 6 ,
    4 , 6 , 7 ,

    8 , 9 , 10 ,
    8 , 10 , 11 ,

    12 , 13 , 14 ,
    12 , 14 , 15 ,

    16 , 17 , 18 ,
    16 , 18 , 19 ,

    20 , 21 , 22 ,
    20 , 22 , 23 ,
};

///> End Data

///> Enum/Class/Struct Definitions

enum ShaderType {
    VERTEX_SHADER = GL_VERTEX_SHADER ,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER ,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER ,
    TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER ,
    TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER ,
    COMPUTE_SHADER = GL_COMPUTE_SHADER ,
};

enum BufferUsage {
    STATIC_DRAW = GL_STATIC_DRAW ,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW ,
    STREAM_DRAW = GL_STREAM_DRAW ,
};

enum BufferType {
    ARRAY_BUFFER = GL_ARRAY_BUFFER ,
    ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER ,
};

struct Window {
    SDL_Window* window;
    SDL_GLContext context;

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    glm::ivec2 size{ 1920 , 1080 };
    glm::vec4 clear_color{ 0.2f , 0.3f , 0.3f , 1.0f };
};

class VertexBuffer {
    BufferType buffer_type;
    BufferUsage buffer_usage; 
    
    uint32_t renderer_id;
    uint32_t buffer_size;
    const void* buffer_data;

    public:
        VertexBuffer(
            const void* data , uint32_t size , 
            BufferUsage usage = STATIC_DRAW , 
            BufferType type = ARRAY_BUFFER
        );
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};

class VertexArray { 
    uint32_t renderer_id;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> layout;

    Scope<VertexBuffer> vertex_buffer = nullptr;
    Scope<VertexBuffer> index_buffer = nullptr;

    void SetLayout();

    public:
        VertexArray(const std::vector<float>& vertices , const std::vector<uint32_t>& indices = {} , const std::vector<uint32_t>& layout = {}) ;

        ~VertexArray();

        void Bind() const;
        void Draw() const;
        void Unbind() const;
};

class Shader {
    uint32_t renderer_id;

    std::string vertex_path;
    std::string fragment_path;

    std::string vertex_src;
    std::string fragment_src;

    std::unordered_map<std::string , int32_t> uniform_locations;

    bool valid = false;

    bool CompileShader(uint32_t shader_piece , const char* src);
    bool LinkShader();

    uint32_t GetUniformLocation(const std::string& name);

    public:
        Shader(const std::string& vertex_path , const std::string& fragment_path);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        inline const bool IsValid() const { return valid; }

        void SetUniform(const std::string& name , const int32_t& value);
        void SetUniform(const std::string& name , const float& value);
        void SetUniform(const std::string& name , const glm::vec2& value);
        void SetUniform(const std::string& name , const glm::vec3& value);
        void SetUniform(const std::string& name , const glm::vec4& value);
        void SetUniform(const std::string& name , const glm::mat2& value);
        void SetUniform(const std::string& name , const glm::mat3& value);
        void SetUniform(const std::string& name , const glm::mat4& value);
};

class Camera {
    float fov;
    float aspect;
    
    glm::vec2 clip;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 up;
    glm::vec3 world_up;
    glm::vec3 front;
    glm::vec3 right;

    glm::mat4 projection;
    glm::mat4 view;

    void Recalculate();

    public:

        Camera();

        inline void SetFOV(float fov) { this->fov = fov; Recalculate(); }
        inline void SetAspect(float aspect) { this->aspect = aspect; Recalculate(); }
        inline void SetClip(const glm::vec2& clip) { this->clip = clip; Recalculate(); }
        inline void SetPosition(const glm::vec3& position) { this->position = position; Recalculate(); }
        inline void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; Recalculate(); }
        inline void SetUp(const glm::vec3& up) { this->up = up; Recalculate(); }
        inline void SetWorldUp(const glm::vec3& world_up) { this->world_up = world_up; Recalculate(); }
        inline void SetFront(const glm::vec3& front) { this->front = front; Recalculate(); }
        inline void SetRight(const glm::vec3& right) { this->right = right; Recalculate(); }

        inline const float FOV() const { return fov; }
        inline const float Aspect() const { return aspect; }
        inline const glm::vec3& Position() const { return position; }
        inline const glm::vec3& Rotation() const { return rotation; }
        inline const glm::vec3& Up() const { return up; }
        inline const glm::vec3& WorldUp() const { return world_up; }
        inline const glm::vec3& Front() const { return front; }
        inline const glm::vec3& Right() const { return right; }
        inline const glm::vec2& Clip() const { return clip; }
        inline const glm::mat4& Projection() const { return projection; }
        inline const glm::mat4& View() const { return view; }
};

///> End Enum/Class/Struct Definitions

///> Function Declarations

Scope<Window> GetWindow();
void DestroyWindow(Scope<Window>& window);
std::string ReadFile(const std::string& file);

///> End Function Declarations

int main() {
    Scope<Window> window = GetWindow();
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }

    VertexArray vao(vertices , indices);
    Shader shader("yockcraft/resources/tri.vert" , "yockcraft/resources/tri.frag");
    if (!shader.IsValid()) {
        std::cout << "Failed to create shader" << std::endl;
        DestroyWindow(window);
        return 1;
    }

    Camera camera;
    camera.SetPosition(glm::vec3(0.0f , 0.0f , 3.0f));

    glm::mat4 model(1.0f);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        model = glm::rotate(model , glm::radians(0.5f) , glm::vec3(0.0f , 1.0f , 0.0f));

        glClearColor(
            window->clear_color.r , window->clear_color.g , 
            window->clear_color.b , window->clear_color.a
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform("projection" , camera.Projection());
        shader.SetUniform("view" , camera.View());
        shader.SetUniform("model" , model);
        vao.Bind();
        vao.Draw();

        SDL_GL_SwapWindow(window->window);
    }

    DestroyWindow(window);

    return 0;
}

///> Class Methods

 VertexBuffer::VertexBuffer(
    const void* data , uint32_t size , 
    BufferUsage usage , 
    BufferType type
) : buffer_type(type) , buffer_usage(usage) , 
        renderer_id(0) , buffer_size(size)  ,
        buffer_data(data) {
    glGenBuffers(1 , &renderer_id);
    glBindBuffer(type , renderer_id);
    glBufferData(type , size , data , usage);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1 , &renderer_id);
}

void VertexBuffer::Bind() const {
    glBindBuffer(buffer_type , renderer_id);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(buffer_type , 0);
}

void VertexArray::SetLayout() {
    if (layout.size() == 0) {
        layout = { 3 };
    }

    uint32_t stride = 0;
    for (uint32_t i = 0; i < layout.size(); i++) {
        stride += layout[i];
    }
    stride *= sizeof(float);

    uint32_t offset = 0;
    for (uint32_t i = 0; i < layout.size(); i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i , layout[i] , GL_FLOAT , GL_FALSE , stride , (void*)(offset * sizeof(float)));

        offset += layout[i];
    }
}

VertexArray::VertexArray(const std::vector<float>& vertices , const std::vector<uint32_t>& indices , const std::vector<uint32_t>& layout)
        : vertices(vertices) , indices(indices) , layout(layout) {
    glGenVertexArrays(1 , &renderer_id);
    glBindVertexArray(renderer_id);

    vertex_buffer = std::make_unique<VertexBuffer>(
        vertices.data() , vertices.size() * sizeof(float)
    );
    
    if (indices.size() == 0) {
        SetLayout();
        return;
    }

    index_buffer = std::make_unique<VertexBuffer>(
        indices.data() , indices.size() * sizeof(uint32_t) , 
        STATIC_DRAW , ELEMENT_ARRAY_BUFFER
    );

    SetLayout();
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1 , &renderer_id);
}

void VertexArray::Bind() const {
    glBindVertexArray(renderer_id);
}

void VertexArray::Draw() const {
    if (index_buffer != nullptr) {
        glDrawElements(GL_TRIANGLES , indices.size() , GL_UNSIGNED_INT , 0);
    } else {
        glDrawArrays(GL_TRIANGLES , 0 , vertices.size());
    }
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

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
    vertex_src = ReadFile(vertex_path);
    fragment_src = ReadFile(fragment_path);

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

void Camera::Recalculate() {
    projection = glm::perspective(glm::radians(fov) , aspect , clip.x , clip.y);
    view = glm::lookAt(position , position + front , up);
}

Camera::Camera() 
        : fov(70.0f) , aspect(16.0f / 9.0f) , clip(0.1f , 100.0f) , position(0.0f) , rotation(0.0f) , 
        up(0.0f , 1.0f , 0.0f) , world_up(0.0f , 1.0f , 0.0f) , front(0.0f , 0.0f , -1.0f) , right(1.0f , 0.0f , 0.0f) ,
        projection(1.0f) , view(1.0f) {
    Recalculate();
}

///> End Class Methods


///> Function Definitions

Scope<Window> GetWindow() {
    Scope<Window> window = std::make_unique<Window>();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    window->window = SDL_CreateWindow(
        "YockCraft" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 
        window->size.x , window->size.y , window->flags
    );
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION , 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION , 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER , 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE , 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE , 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL , 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS , 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES , 16);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE , 1);

    window->context = SDL_GL_CreateContext(window->window);
    if (window->window == nullptr) {
        std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_MULTISAMPLE);
    // 
    // glEnable(GL_CULL_FACE);

    return window;
}

void DestroyWindow(Scope<Window>& window) {
    if (window != nullptr) {
        SDL_GL_DeleteContext(window->context);
        SDL_DestroyWindow(window->window);
        SDL_Quit();
    }
}

std::string ReadFile(const std::string& file) {
    std::ifstream stream(file);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

///> End Function Definitions

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}

