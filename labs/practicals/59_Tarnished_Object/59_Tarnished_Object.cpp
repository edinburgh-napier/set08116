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
texture tarnish;
target_camera cam;

bool load_content() {
  // *********************************
  // Create a sphere

  // Create box geometry for skybox

  // Scale box by 100 - allows a distance

  // Load the cubemap  - create array of six filenames +x, -x, +y, -y, +z, -z


  // Create cube_map

  // Load in tarnish texture

  // Load in environment map shader


  // Build effect

  // Load in skybox effect


  // Build effect

  // *********************************
  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
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

  // Bind tarnish to TU 1

  // Set tarnish uniform to 1

  // *********************************
  // Render mesh
  renderer::render(sphere);

  return true;
}

void main() {
  // Create application
  app application("59_Tarnished_Object");
  // Set methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}