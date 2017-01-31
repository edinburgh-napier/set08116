#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh cylinder;
effect eff;
texture tex;
texture normal_map;
target_camera cam;
directional_light light;
bool load_content() {
  // Create a cylinder
  cylinder = mesh(geometry_builder::create_cylinder(100, 100));
  // Scale cylinder
  cylinder.get_transform().scale = vec3(5.0f, 5.0f, 5.0f);

  // Load brick.jpg texture
  tex = texture("textures/brick.jpg");
  // Load brick_normalmap.jpg texture
  normal_map = texture("textures/brick_normalmap.jpg");

  // ****************************
  // Set material
  // - emissive black - diffuse (0.53, 0.45, 0.37)
  // - specular white - shininess 25
  // ****************************
  cylinder.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  cylinder.get_material().set_diffuse(vec4(0.53f, 0.45f, 0.37f, 1.0f));
  cylinder.get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  cylinder.get_material().set_shininess(25.0f);

  // Set light properties
  light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
  light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  light.set_direction(normalize(vec3(1.0f, 1.0f, 0.0f)));

  // Load in shaders
  eff.add_shader("55_Normal_Mapping/shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("55_Normal_Mapping/shader.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 2.0f, 15.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
    cylinder.get_transform().rotate(vec3(0.0f, -pi<float>(), 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
    cylinder.get_transform().rotate(vec3(0.0f, pi<float>(), 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    cylinder.get_transform().rotate(vec3(pi<float>(), 0.0f, 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    cylinder.get_transform().rotate(vec3(-pi<float>(), 0.0f, 0.0f) * delta_time);
  }

  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  auto M = cylinder.get_transform().get_transform_matrix();
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Set M matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
  // Set N matrix uniform
  glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE,
                     value_ptr(cylinder.get_transform().get_normal_matrix()));
  // *********************************
  // Bind material

  // Bind light

  // Bind texture

  // Set tex uniform

  // Bind normal_map

  // Set normal_map uniform

  // Set eye position

  // Render mesh

  // *********************************
  return true;
}

void main() {
  // Create application
  app application("55_Normal_Mapping");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}