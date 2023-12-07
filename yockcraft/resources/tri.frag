#version 460 core

in vec4 frag_pos;
in vec4 frag_color;

out vec4 FragColor;

void main() {
    FragColor = frag_pos;
}
