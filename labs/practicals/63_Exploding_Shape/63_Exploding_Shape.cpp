#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
effect eff;
target_camera cam;
float explode_factor = 0.0f;

bool load_content() {
  // Create a cylinder
  sphere = mesh(geometry_builder::create_sphere(100, 100));
  //sphere = mesh(geometry_builder::create_box());
  // Scale cylinder
  sphere.get_transform().scale = vec3(5.0f, 5.0f, 5.0f);

  // Load in shaders
  eff.add_shader("63_Exploding_Shape/shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("63_Exploding_Shape/shader.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("63_Exploding_Shape/explode.geom", GL_GEOMETRY_SHADER);

  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 2.0f, 50.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.01f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
    sphere.get_transform().rotate(vec3(0.0f, -pi<float>(), 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
    sphere.get_transform().rotate(vec3(0.0f, pi<float>(), 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    sphere.get_transform().rotate(vec3(pi<float>(), 0.0f, 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    sphere.get_transform().rotate(vec3(-pi<float>(), 0.0f, 0.0f) * delta_time);
  }
  // *********************************
  // Use o and p to modify explode factor =/- 0.1f






  // *********************************
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
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // *********************************
  // Set explode factor uniform

  // *********************************
  // Render mesh
  //glDisable(GL_CULL_FACE); //turn this on to see cool stuff.
  renderer::render(sphere);

  return true;
}

void main() {
  // Create application
  app application("63_Exploding_Shape");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}