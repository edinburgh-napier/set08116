#version 440

// MVP transformation
uniform mat4 MVP;

// Vertex position
layout(location = 0) in vec3 position;

void main() {
  // Pass through position to geometry shader
  gl_Position = MVP * vec4(position, 1.0);
}