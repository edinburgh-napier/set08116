#version 440

uniform sampler2D tex;
uniform sampler2D dissolve;

// *********************************
// Declare dissolve factor value

// *********************************

// Incoming texture coordinates
layout(location = 0) in vec2 tex_coord;
// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Get dissolve value from the dissolve texture

  // If r component is greater than dissolve factor, discard



  // *********************************

  // Get texture colour
  colour = texture(tex, tex_coord);
}