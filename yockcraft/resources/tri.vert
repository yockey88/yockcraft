#version 460 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec3 vs_color;

out vec4 frag_pos;
out vec4 frag_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    frag_pos = vec4(vs_pos , 1.0);
    frag_color = vec4(vs_color , 1.0);
    gl_Position = projection * view * model * vec4(vs_pos , 1.0);
}
