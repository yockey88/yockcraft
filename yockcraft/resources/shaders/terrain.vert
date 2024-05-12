#version 460 core

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_texcoords;

out vec4 frag_pos;
out vec2 frag_texcoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    frag_pos = vec4(vs_pos , 1.0);
    frag_texcoords = vs_texcoords;

    gl_Position = projection * view * mat4(1.0) * vs_pos;
}
