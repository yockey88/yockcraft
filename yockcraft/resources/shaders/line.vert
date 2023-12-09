#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 frag_color;

uniform vec3 uni_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    frag_color = vec4(uni_color , 1.0);
    gl_Position = projection * view * model * vec4(pos , 1.0);
}
