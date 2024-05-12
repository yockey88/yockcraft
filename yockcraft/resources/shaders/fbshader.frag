#version 460 core

in vec2 fs_uv;

out vec4 FinalColor;

uniform sampler2D fb_tex;

void main() {
  vec3 col = texture(fb_tex , fs_uv).rgb;
  FinalColor = vec4(col , 1.0);
}
