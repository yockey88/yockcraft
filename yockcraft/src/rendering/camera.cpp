#include "rendering/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace yockcraft {

void Camera::Recalculate() {
  projection = glm::perspective(glm::radians(fov), aspect, clip.x, clip.y);
  view = glm::lookAt(position, position + front, up);
}

Camera::Camera()
    : constrain_pitch(true),
      fov(70.0f),
      aspect(16.0f / 9.0f),
      sensitivity(0.2f),
      speed(0.1f),
      mouse_pos(0.0f),
      mouse_delta(0.0f),
      clip(0.1f, 1000.0f),
      position(0.0f),
      rotation(glm::vec3(-90.f, 0.f, 0.f)),
      up(0.0f, 1.0f, 0.0f),
      world_up(0.0f, 1.0f, 0.0f),
      front(0.0f, 0.0f, -1.0f),
      right(1.0f, 0.0f, 0.0f),
      projection(1.0f),
      view(1.0f) {
  Recalculate();
}

}  // namespace yockcraft
