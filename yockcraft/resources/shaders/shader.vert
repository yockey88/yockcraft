#version 460 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec3 vs_normal;
layout (location = 2) in vec2 vs_texcoords;

out vec3 frag_pos;
out vec2 frag_texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  frag_pos = vs_pos;
  frag_texcoords = vs_texcoords;
  gl_Position = projection * view * model * vec4(vs_pos , 1.0);
}
