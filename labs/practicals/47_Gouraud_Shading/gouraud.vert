#version 440

// A directional light structure
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};

// A material structure
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;
// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;

// Outgoing primary colour
layout(location = 0) out vec4 primary;
// Outgoing secondary colour
layout(location = 1) out vec4 secondary;
// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord_out;

void main() {
  // *********************************
  // Calculate position

  // Calculate ambient component

  // Transform the normal

  // Calculate k

  // Calculate diffuse

  // Calculate world position of vertex

  // Calculate view direction

  // Calculate half vector between view_dir and light_dir

  // Calculate specular component
  // Calculate k

  // Calculate specular

  // Set primary

  // Set secondary

  // Ensure primary and secondary alphas are 1


  // Pass through texture coordinate

  // *********************************
}