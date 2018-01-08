#include "graphics_framework.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
geometry geom2;
geometry geom3;
geometry geom4;
effect eff;
effect teff;
effect sbeff;
texture tpng;
texture tjpg;
target_camera cam;
cubemap cube_map;
float theta = 0.0f;


bool load_content() {

  vector<vec3> positions{vec3(0.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f)};
  vector<vec4> colours{vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)};
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  geom2 = geometry_builder::create_plane(10, 10);
  geom4 = geometry_builder::create_box();

  // Load in model
  auto src = "models/icosphere.obj";
  geom3 = geometry(src);

  src = "textures/sign.jpg";
  tpng = texture(src, false, false);
  src = "textures/stonygrass.jpg";
  tjpg = texture(src, false, false);

  array<string, 6> filenames = {"textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
                                "textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg"};
  cube_map = cubemap(filenames);
  // Load in shaders
  eff.add_shader("shaders/basic.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic.frag", GL_FRAGMENT_SHADER);
  eff.build();
  teff.add_shader("shaders/basic_textured.vert", GL_VERTEX_SHADER);
  teff.add_shader("shaders/basic_textured.frag", GL_FRAGMENT_SHADER);
  teff.build();
  sbeff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
  sbeff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
  sbeff.build();
  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(1.0472f, aspect, 2.414f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Update the angle - half rotation per second
  theta += pi<float>() * delta_time;
  // Update the camera
  cam.set_position(rotate(vec3(15.0f, 12.0f, 15.0f), theta * 0.05f, vec3(0, 1.0f, 0)));
  cam.update(delta_time);

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_F)) {
    renderer::set_screen_dimensions(1280, 720);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(1.0472f, aspect, 2.414f, 1000.0f);
  }

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_G)) {
    renderer::set_screen_dimensions(800, 600);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  }

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_H)) {
    renderer::set_screen_mode(renderer::borderless);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(1.0472f, aspect, 2.414f, 1000.0f);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_J)) {
    renderer::set_screen_mode(renderer::fullscreen);
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(1.0472f, aspect, 2.414f, 1000.0f);
  }

  return true;
}

bool render() {
  renderer::bind(eff);
  mat4 R;
  R = rotate(mat4(1.0f), theta, vec3(0.0f, 0.0f, 1.0f));
  mat4 M = R;
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

  renderer::render(geom);
  renderer::render(geom3);
  //

  renderer::bind(teff);
  renderer::bind(tjpg, 0);
  renderer::bind(tpng, 1);
  glUniform1i(teff.get_uniform_location("tex"), 0);
  glUniformMatrix4fv(teff.get_uniform_location("MVP"), 1, GL_FALSE,
                     value_ptr(P * V * translate(mat4(1.0f), vec3(3.0f, -5.0f, -8.0f))));
  renderer::render(geom2);
  glUniform1i(teff.get_uniform_location("tex"), 1);
  glUniformMatrix4fv(teff.get_uniform_location("MVP"), 1, GL_FALSE,
                     value_ptr(P * V * translate(mat4(1.0f), vec3(-8.0f, -5.0f, -8.0f))));
  renderer::render(geom2);

  glDisable(GL_CULL_FACE);

  renderer::bind(sbeff);
  MVP = P * V * scale(mat4(1.0f), vec3(100.0f));
  glUniformMatrix4fv(sbeff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  renderer::bind(cube_map, 0);
  glUniform1i(sbeff.get_uniform_location("cubemap"), 0);
  renderer::render(geom4);

  glEnable(GL_CULL_FACE);

  return true;
}

int main() {
  // Create application
  app application("Welcome to Computer Graphics!", renderer::windowed);
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
  return 0;
}