#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
mesh skybox;
effect eff;
effect sky_eff;
cubemap cube_map;
target_camera cam;

bool load_content() {
  // Create a sphere
  sphere = mesh(geometry_builder::create_sphere(25, 25));
  // Create box geometry for skybox
  skybox = mesh(geometry_builder::create_box());
  // Scale box by 100 - allows a distance
  skybox.get_transform().scale = vec3(100.0f);
  // Load the cubemap  - create array of six filenames +x, -x, +y, -y, +z, -z
  array<string, 6> filenames = {"textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
                                "textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg"};
  // Create cube_map
  cube_map = cubemap(filenames);
  // Load in environment map shader
  eff.add_shader("58_Environment_Maps/env_map.vert", GL_VERTEX_SHADER);
  eff.add_shader("58_Environment_Maps/env_map.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();
  // Load in skybox effect
  sky_eff.add_shader("57_Skybox/shader.vert", GL_VERTEX_SHADER);
  sky_eff.add_shader("57_Skybox/shader.frag", GL_FRAGMENT_SHADER);
  // Build effect
  sky_eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  cam.update(delta_time);
  // *********************************
  // Set skybox position to camera position (camera in centre of skybox)

  // rotate the sphere

  // *********************************
  return true;
}

bool render() {
  // *********************************
  // Disable depth test and depth mask


  // Bind skybox effect

  // Calculate MVP for the skybox




  // Set MVP matrix uniform

  // Bind cubemap to TU 0

  // Set cubemap uniform

  // Render skybox

  // Enable depth test and depth mask



  // Bind effect

  // Create MVP matrix




  // Set MVP matrix uniform

  // Set M matrix uniform

  // Set N matrix uniform

  // Set eye_pos value

  // Bind cubemap to TU 0

  // Set cubemap uniform

  // Render mesh

  // *********************************
  return true;
}

void main() {
  // Create application
  app application("58_Environment_Maps");
  // Set methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}