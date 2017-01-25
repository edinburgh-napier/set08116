#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m;
effect eff;
target_camera cam;
// Main texture
texture tex;
// Dissolve texture
texture dissolve;
// Dissolve factor to set on shader
float dissolve_factor = 1.0f;
vec2 uv_scroll;

bool load_content() {
  // Create mesh object, cheating and using the mesh builder for now
  m = mesh(geometry_builder::create_box());
  // Scale geometry
  m.get_transform().scale = vec3(10.0f);

  // Load in dissolve shader
  eff.add_shader("36_Dissolve/dissolve.vert", GL_VERTEX_SHADER);
  eff.add_shader("36_Dissolve/dissolve.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Load in textures
  tex = texture("textures/checker.png");
  dissolve = texture("textures/blend_map2.jpg");

  // Set camera properties
  cam.set_position(vec3(30.0f, 30.0f, 30.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

  return true;
}

bool update(float delta_time) {
  // Use up an down to modify the dissolve factor
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
    dissolve_factor = clamp(dissolve_factor + 0.1f * delta_time, 0.0f, 1.0f);
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
    dissolve_factor = clamp(dissolve_factor - 0.1f * delta_time, 0.0f, 1.0f);
  // Update camera
  cam.update(delta_time);
  uv_scroll += vec2(0, delta_time * 0.05);
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
  // Set the dissolve_factor uniform value

  // Bind the two textures - use different index for each


  // Set the uniform values for textures - use correct index


  // *********************************

  // Set UV_scroll uniform, adds cool movent (Protip: This is a super easy way to do fire effects;))
  glUniform2fv(eff.get_uniform_location("UV_SCROLL"), 1, value_ptr(uv_scroll));
  // Render the mesh
  renderer::render(m);

  return true;
}

void main() {
  // Create application
  app application("33_Dissolve");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}