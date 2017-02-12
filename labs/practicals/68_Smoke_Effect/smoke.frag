#version 440 core

uniform sampler2D tex;

in VertexData {
  vec4 colour;
  vec2 tex_coord;
};

layout(location = 0) out vec4 colour_out;

void main() { colour_out = texture(tex, tex_coord) * colour; }