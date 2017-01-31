#version 440

// MVP transformation
uniform mat4 MVP;
// MV transformation
uniform mat4 MV;
// M transformation
uniform mat4 M;
// N transformation
uniform mat3 N;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming tex_coord
layout(location = 10) in vec2 tex_coord;

// Outgoing world position
layout(location = 0) out vec3 vertex_position;
// Camera space position
layout(location = 1) out vec4 CS_position;
// Transformed normal
layout(location = 2) out vec3 transformed_normal;
// Outgoing tex_coord
layout(location = 3) out vec2 vertex_tex_coord;

void main() {
  // Calculate screen space coordinate
  gl_Position = MVP * vec4(position, 1.0);
  // Calculate world position
  vertex_position = (M * vec4(position, 1.0)).xyz;

  // *********************************
  // Calculate camera space coordinate
  // *********************************
  CS_position = MV * vec4(position, 1.0);

  // Transform normal
  transformed_normal = N * normal;
  // Pass through tex_coord
  vertex_tex_coord = tex_coord;
}