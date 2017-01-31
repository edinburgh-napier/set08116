#version 440

// Texture to use on billboards
uniform sampler2D tex;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // Colour is just texture colour
  colour = texture(tex, tex_coord);
}