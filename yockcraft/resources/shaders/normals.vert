#version 460 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec3 vs_normal;

out VS_OUT {
  vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;

void main() {
  mat3 normal_mat = mat3(transpose(inverse(view * model)));
  vs_out.normal = normalize(vec3(vec4(normal_mat * vs_normal , 0.0)));
  gl_Position = view * model * vec4(vs_pos , 1.0);
}
