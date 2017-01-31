#version 440

// This shader requires direction.frag, point.frag and spot.frag

// Directional light structure
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};
#endif

// Point light information
#ifndef POINT_LIGHT
#define POINT_LIGHT
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};
#endif

// Spot light data
#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};
#endif

// A material structure
#ifndef MATERIAL
#define MATERIAL
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};
#endif

// Forward declarations of used functions
vec4 calculate_direction(in directional_light light, in material mat, in vec3 normal, in vec3 view_dir,
                         in vec4 tex_colour);
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour);
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                    in vec4 tex_colour);

// Directional light information
uniform directional_light light;
// Point lights being used in the scene
uniform point_light points[4];
// Spot lights being used in the scene
uniform spot_light spots[5];
// Material of the object being rendered
uniform material mat;
// Position of the eye
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Calculate view direction

  // Sample texture

  // Calculate directional light colour

  // Sum point lights



  // Sum spot lights



  // *********************************
}