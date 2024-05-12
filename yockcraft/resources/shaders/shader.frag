#version 460 core

in vec3 frag_pos;
in vec2 frag_texcoords;

out vec4 FragColor;

uniform sampler2D atlas;

void main() {
  // FragColor = vec4(vec3(frag_pos.y * 6 / 255) , 1.0); 
  FragColor = texture(atlas , frag_texcoords);
}
