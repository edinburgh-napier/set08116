#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
effect eff;
cubemap cube_map;
target_camera cam;

bool load_content() {
  // Create a sphere
  sphere = mesh(geometry_builder::create_sphere(25, 25));

  array<string, 6> filenames = {"textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
                                "textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg"};
  // *********************************
  // Create cube_map

  // Load in shaders


  // Build effect

  // *********************************

  // Set camera properties
  cam.set_position(vec3(0.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  auto M = sphere.get_transform().get_transform_matrix();
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  renderer::bind(cube_map, 0);
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Render mesh
  renderer::render(sphere);

  return true;
}

void main() {
  // Create application
  app application("56_Cube_Maps");
  // Set methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}