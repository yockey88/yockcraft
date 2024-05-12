#version 460 core

out vec4 FragColor;

uniform vec3 uni_color;

void main() {
    FragColor = vec4(uni_color , 1.0); 
}
