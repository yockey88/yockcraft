#version 460 core

layout (location = 0) in vec3 vs_pos;

out vec4 frag_pos;
out vec3 frag_texcoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    frag_pos = vec4(vs_pos , 1.0);
    frag_texcoords = vs_pos;
    gl_Position = projection * view * model * vec4(vs_pos , 1.0);
}
