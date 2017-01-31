#version 440

// Requires direction.frag and fog.frag

// Directional light structure
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
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

// Forward declarations
vec4 calculate_direction(in directional_light light, in material mat, in vec3 normal, in vec3 view_dir,
                         in vec4 tex_colour);
float calculate_fog(in float fog_coord, in vec4 fog_colour, in float fog_start, in float fog_end, in float fog_density,
                    in int fog_type);

// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;
// Fog colour
uniform vec4 fog_colour;
// Fog start position
uniform float fog_start;
// Fog end position
uniform float fog_end;
// Fog density
uniform float fog_density;
// Fog type
uniform int fog_type;

// Incoming world position
layout(location = 0) in vec3 position;
// Camera space position
layout(location = 1) in vec4 CS_position;
// Transformed normal
layout(location = 2) in vec3 normal;
// Incoming tex_coord
layout(location = 3) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // Calculate view direction
  vec3 view_dir = normalize(eye_pos - position);
  // Sample texture
  vec4 tex_colour = texture(tex, tex_coord);
  // Calculate colour for direction light
  colour = calculate_direction(light, mat, normal, view_dir, tex_colour);
  colour.a = 1.0;

  // *********************************
  // Calculate fog coord
  // - convert from homogeneous
  // - ensure value is positive (we want the size of the value)


  // Calculate fog factor


  // Colour is mix between colour and fog colour based on factor

  // *********************************
}