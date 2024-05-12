#version 460 core

in vec4 frag_pos;
in vec2 frag_texcoords;

out vec3 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex , frag_texcoords); 
}
