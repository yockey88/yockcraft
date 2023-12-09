#include "yockcraft.hpp"

void CameraKeyboardCallback(yockcraft::Camera* camera);
void CameraMouseCallback(yockcraft::Camera* camera);

static const std::vector<float> vertices = {
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

static const std::vector<float> xaxis {
    -16.f , 0.f , 0.f ,
     16.f , 0.f , 0.f ,
};

static const std::vector<float> yaxis {
    0.f , -16.f , 0.f ,
    0.f ,  16.f , 0.f ,
};

static const std::vector<float> zaxis {
    0.f , 0.f , -16.f , 
    0.f , 0.f ,  16.f , 
};

int main() {
    yockcraft::State state;

    Scope<yockcraft::Window> window = yockcraft::GetWindow();
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForOpenGL(window->window , window->context);
    ImGui_ImplOpenGL3_Init("#version 460");


    {

        yockcraft::VertexArray cube(vertices , indices);
        yockcraft::VertexArray xaxis_vao(xaxis);
        yockcraft::VertexArray yaxis_vao(yaxis);
        yockcraft::VertexArray zaxis_vao(zaxis);

        yockcraft::Shader shader("yockcraft/resources/shaders/shader.vert" , "yockcraft/resources/shaders/shader.frag");
        if (!shader.IsValid()) {
            std::cout << "Failed to create shader" << std::endl;
            DestroyWindow(window);
            return 1;
        }
        yockcraft::Texture texture(cube_map_faces);

        yockcraft::Shader line_shader("yockcraft/resources/shaders/line.vert" , "yockcraft/resources/shaders/line.frag");
        if (!line_shader.IsValid()) {
            std::cout << "Failed to create line shader" << std::endl;
            DestroyWindow(window);
            return 1;
        }

        yockcraft::Chunk chunk;

        yockcraft::Camera camera;
        camera.SetPosition(glm::vec3(0.0f , 0.0f , 3.0f));
        
        glm::mat4 model = glm::mat4(1.0f);

        std::chrono::time_point<std::chrono::system_clock> now , last;

        while (state.running) {
            last = now;
            now = std::chrono::system_clock::now();
            std::chrono::duration<float> delta = now - last;
            
            PollEvents(window , state);

            glPolygonMode(GL_FRONT_AND_BACK , state.render_mode);

            if (state.camera_free) {
                CameraKeyboardCallback(&camera);
                CameraMouseCallback(&camera);
            }
            
            model = glm::rotate(model , glm::radians(0.5f) , glm::vec3(0.0f , 0.0f , 1.0f));

            glClearColor(
                window->clear_color.r , window->clear_color.g , 
                window->clear_color.b , window->clear_color.a
            );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window->window);
            ImGui::NewFrame();

            texture.Bind();
            shader.Bind();
            shader.SetUniform("projection" , camera.Projection());
            shader.SetUniform("view" , camera.View());
            shader.SetUniform("model" , model);
            shader.SetUniform("uni_color" , glm::vec3(1.0f , 1.0f , 1.0f));
            chunk.Draw(&shader);

            line_shader.Bind();
            line_shader.SetUniform("projection" , camera.Projection());
            line_shader.SetUniform("view" , camera.View());
            line_shader.SetUniform("model" , glm::mat4(1.0f));
            line_shader.SetUniform("uni_color" , glm::vec3(1.0f , 0.0f , 0.0f));
            xaxis_vao.Draw(yockcraft::DrawMode::LINES);
            line_shader.SetUniform("uni_color" , glm::vec3(0.0f , 1.0f , 0.0f));
            yaxis_vao.Draw(yockcraft::DrawMode::LINES);
            line_shader.SetUniform("uni_color" , glm::vec3(0.0f , 0.0f , 1.0f));
            zaxis_vao.Draw(yockcraft::DrawMode::LINES);

            if (ImGui::Begin("Framerate")) {
                ImGui::Text("FPS :: [%f]" , 1.0f / delta.count());
            }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            SDL_GL_MakeCurrent(window->window , window->context);
            SDL_GL_SwapWindow(window->window);
        }
    }

    ImGui::DestroyContext();

    yockcraft::DestroyWindow(window);

    return 0;
}

void CameraKeyboardCallback(yockcraft::Camera* camera) {
    float camera_speed = camera->Speed();
    glm::vec3 camera_position = camera->Position();

    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W]) {
        camera_position += camera_speed * camera->Front();
    }
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_S]) {
        camera_position -= camera_speed * camera->Front();
    }
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A]) {
        camera_position -= glm::normalize(glm::cross(camera->Front() , camera->Up())) * camera_speed;
    }
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D]) {
        camera_position += glm::normalize(glm::cross(camera->Front() , camera->Up())) * camera_speed;
    }
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]) {
        camera_position += camera_speed * camera->Up();
    }
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LSHIFT]) {
        camera_position -= camera_speed * camera->Up();
    }

    camera->SetPosition(camera_position);
}

void CameraMouseCallback(yockcraft::Camera* camera) {
    SDL_WarpMouseInWindow(nullptr , 1920 / 2 , 1080 / 2); 

    int x, y;
    SDL_GetGlobalMouseState(&x, &y);

    camera->SetLastMousePos(camera->MousePos());
    camera->SetMousePos({ x , y });
    camera->SetMouseDelta({
        camera->MousePos().x - camera->LastMousePos().x ,
        camera->LastMousePos().y - camera->MousePos().y 
    });

    int rel_x, rel_y;
    SDL_GetRelativeMouseState(&rel_x, &rel_y);

    camera->SetYaw(camera->Yaw() + (rel_x * camera->Sensitivity()));
    camera->SetPitch(camera->Pitch() - (rel_y * camera->Sensitivity()));

    if (camera->ConstrainPitch()) {
        if (camera->Pitch() > 89.0f) 
            camera->SetPitch(89.0f);
        if (camera->Pitch() < -89.0f) 
            camera->SetPitch(-89.0f);
    }

    glm::vec3 front;
    front.x = cos(glm::radians(camera->Yaw())) * cos(glm::radians(camera->Pitch()));
    front.y = sin(glm::radians(camera->Pitch()));
    front.z = sin(glm::radians(camera->Yaw())) * cos(glm::radians(camera->Pitch()));
    camera->SetFront(glm::normalize(front));
    camera->SetRight(glm::normalize(glm::cross(camera->Front() , camera->WorldUp())));
    camera->SetUp(glm::normalize(glm::cross(camera->Right() , camera->Front())));
    camera->SetPosition(camera->Position());
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}

