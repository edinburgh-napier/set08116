#version 440

// Model-view transformation
uniform mat4 MV;

// Incoming position data
layout(location = 0) in vec3 position;

void main() {
  // Transform position into camera space
  gl_Position = MV * vec4(position, 1.0);
}