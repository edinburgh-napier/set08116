#version 440

// The model transformation
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// The ambient intensity of the scene
uniform vec4 ambient_intensity;
// The light colour of the scene
uniform vec4 light_colour;
// Direction of the light
uniform vec3 light_dir;
// The diffuse reflection colour
uniform vec4 diffuse_reflection;
// The specular reflection colour
uniform vec4 specular_reflection;
// Shininess of the object
uniform float shininess;
// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Outgoing vertex colour
layout(location = 0) out vec4 vertex_colour;

void main() {
  // Calculate position

  // Calculate ambient component

  // Transform the normal

  // Calculate k

  // Calculate diffuse

  // Calculate world position of vertex

  // Calculate view direction

  // Calculate half vector between view_dir and light_dir

  // Calculate k

  // Calculate specular

  // Output combined components

  // *********************************
  // Ensure alpha is 1
  vertex_colour.a = 1.0;
}