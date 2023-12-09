#version 460 core

in vec4 frag_pos;
in vec3 frag_texcoords;

out vec4 FragColor;

uniform samplerCube tex;

void main() {
    FragColor = texture(tex, frag_texcoords); 
}
