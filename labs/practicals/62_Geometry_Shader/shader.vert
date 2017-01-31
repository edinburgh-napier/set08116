#version 440

// Vertex position
layout(location = 0) in vec3 position;

void main() {
  // Pass through position to geometry shader
  gl_Position = vec4(position, 1.0);
}