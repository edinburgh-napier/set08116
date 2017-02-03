#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
effect eff;
target_camera cam;
// Main textures
array<texture, 2> texs;
// Blend map
texture blend_map;

bool load_content() {
  // Construct geometry object
  geometry geom;
  geom.set_type(GL_TRIANGLE_STRIP);

  // Positions
  vector<vec3> positions{vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
                         vec3(1.0f, 1.0f, 0.0f)};
  // Texture coordinates
  vector<vec2> tex_coords{vec2(0.0f, 0.0f), vec2(2.0f, 0.0f), vec2(0.0f, 2.0f), vec2(2.0f, 2.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Create mesh object
  m = mesh(geom);
  // Scale geometry
  m.get_transform().scale = vec3(10.0f, 10.0f, 10.0f);

  // Load in blend shader
  eff.add_shader("32_Blended_Textures/blend.vert", GL_VERTEX_SHADER);
  eff.add_shader("32_Blended_Textures/blend.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Load main two textures
  texs[0] = texture("textures/grass.jpg");
  texs[1] = texture("textures/stonygrass.jpg");
  // Load blend map, try both blend_map1.png and blend_map2.jpg
  blend_map = texture("textures/blend_map1.png");

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 30.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

  return true;
}

bool update(float delta_time) {
  // Update camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  auto M = m.get_transform().get_transform_matrix();
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
                     1,                               // Number of values - 1 mat4
                     GL_FALSE,                        // Transpose the matrix?
                     value_ptr(MVP));                 // Pointer to matrix data

  // *********************************
  // Bind the three textures - use different index for each



  // *********************************

  // Set the uniform values for textures
  static int tex_indices[] = {0, 1};
  glUniform1iv(eff.get_uniform_location("tex"), 2, tex_indices);
  glUniform1i(eff.get_uniform_location("blend"), 2);

  // Render the mesh
  renderer::render(m);

  return true;
}

void main() {
  // Create application
  app application("32_Blended_Textures");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}