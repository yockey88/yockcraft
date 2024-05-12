#ifndef YOCKCRAFT_CAMERA_HPP
#define YOCKCRAFT_CAMERA_HPP

#include <glm/glm.hpp>

namespace yockcraft {

    class Camera {
        bool constrain_pitch;

        float fov;
        float aspect;
        float sensitivity;
        float speed;
    
        glm::vec2 mouse_pos;
        glm::vec2 last_mouse_pos;
        glm::vec2 mouse_delta;
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
            
            inline void SetConstrainPitch(bool constrain_pitch) { this->constrain_pitch = constrain_pitch; }
            inline void SetFOV(float fov) { this->fov = fov; Recalculate(); }
            inline void SetAspect(float aspect) { this->aspect = aspect; Recalculate(); }
            inline void SetSensitivity(float sensitivity) { this->sensitivity = sensitivity; }
            inline void SetSpeed(float speed) { this->speed = speed; }
            inline void SetYaw(float yaw) { rotation.x = yaw; }
            inline void SetPitch(float pitch) { rotation.y = pitch; }
            inline void SetRoll(float roll) { rotation.z = roll; }
            inline void SetClip(const glm::vec2 &clip) { this->clip = clip; Recalculate(); }
            inline void SetMousePos(const glm::vec2 &mouse_pos) { this->mouse_pos = mouse_pos; }
            inline void SetLastMousePos(const glm::vec2 &last_mouse_pos) { this->last_mouse_pos = last_mouse_pos; }
            inline void SetMouseDelta(const glm::vec2 &mouse_delta) { this->mouse_delta = mouse_delta; }
            inline void SetPosition(const glm::vec3 &position) { this->position = position; Recalculate(); }
            inline void SetRotation(const glm::vec3 &rotation) { this->rotation = rotation; }
            inline void SetUp(const glm::vec3 &up) { this->up = up; Recalculate(); }
            inline void SetWorldUp(const glm::vec3 &world_up) { this->world_up = world_up; }
            inline void SetFront(const glm::vec3 &front) { this->front = front; Recalculate(); }
            inline void SetRight(const glm::vec3 &right) { this->right = right; }
            
            inline const bool ConstrainPitch() const { return constrain_pitch; }
            inline const float FOV() const { return fov; }
            inline const float Aspect() const { return aspect; }
            inline const float Sensitivity() const { return sensitivity; }
            inline const float Speed() const { return speed; }
            inline const float Yaw() const { return rotation.x; }
            inline const float Pitch() const { return rotation.y; }
            inline const float Roll() const { return rotation.y; }
            inline const float ClipNear() const { return clip.x; }
            inline const float ClipFar() const { return clip.y; }
            inline const glm::vec2& Clip() const { return clip; }
            inline const glm::vec2& MousePos() const { return mouse_pos; }
            inline const glm::vec2& LastMousePos() const { return last_mouse_pos; }
            inline const glm::vec2& MouseDelta() const { return mouse_delta; }
            inline const glm::vec3& Position() const { return position; }
            inline const glm::vec3& Rotation() const { return rotation; }
            inline const glm::vec3& Up() const { return up; }
            inline const glm::vec3& WorldUp() const { return world_up; }
            inline const glm::vec3& Front() const { return front; }
            inline const glm::vec3& Right() const { return right; }
            inline const glm::mat4& Projection() const { return projection; }
            inline const glm::mat4& View() const { return view; }
    };

} // namespace yockcraft

#endif // !YOCKCRAFT_CAMERA_HPP
