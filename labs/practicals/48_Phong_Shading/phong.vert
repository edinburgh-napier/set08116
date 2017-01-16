#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming texture coordinates
layout(location = 10) in vec2 tex_coord_in;

// Outgoing position
layout(location = 0) out vec3 vertex_position;
// Outgoing normal
layout(location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord_out;

void main() {
  // Set position
  gl_Position = MVP * vec4(position, 1);
  // *********************************
  // Output other values to fragment shader



  // *********************************
}