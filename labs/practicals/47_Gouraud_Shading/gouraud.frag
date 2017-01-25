#version 440

// Texture to sample from
uniform sampler2D tex;

// Incoming primary colour
layout(location = 0) in vec4 primary;
// Incoming secondary colour
layout(location = 1) in vec4 secondary;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample texture

  // Calculate colour

  // *********************************
}