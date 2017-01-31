#version 440

// Requires weighted_texture.frag

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

// Forward declaration
vec4 weighted_texture(in sampler2D tex[4], in vec2 tex_coord, in vec4 weights);

// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Textures
uniform sampler2D tex[4];

// Incoming vertex position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming tex_coord
layout(location = 2) in vec2 tex_coord;
// Incoming tex_weight
layout(location = 3) in vec4 tex_weight;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // Calculate ambient component
  vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;
  // Calculate diffuse component
  vec4 diffuse = (mat.diffuse_reflection * light.light_colour) * max(dot(normal, light.light_dir), 0);
  // Calculate view direction
  vec3 view_dir = normalize(eye_pos - position);
  // Calculate half vector
  vec3 half_vector = normalize(light.light_dir + view_dir);
  // Calculate specular component
  vec4 specular = (mat.specular_reflection * light.light_colour) * pow(max(dot(normal, half_vector), 0), mat.shininess);

  // Get tex colour
  vec4 tex_colour = weighted_texture(tex, tex_coord, tex_weight);

  // Calculate primary colour component
  vec4 primary = mat.emissive + ambient + diffuse;
  // Calculate final colour
  colour = primary * tex_colour + specular;
  colour.a = 1.0;
}