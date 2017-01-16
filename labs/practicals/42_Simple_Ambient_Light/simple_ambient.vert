#version 440

// The transformation matrix
uniform mat4 MVP;
// The ambient intensity of the scene
uniform vec4 ambient_intensity;
// The material colour of the object
uniform vec4 material_colour;

// Incoming position data
layout(location = 0) in vec3 position;

// The outgoing vertex colour
layout(location = 0) out vec4 vertex_colour;

void main() {
  // *********************************
  // Calculate position

  // Calculate ambient component

  // Output vertex colour

  // *********************************
}