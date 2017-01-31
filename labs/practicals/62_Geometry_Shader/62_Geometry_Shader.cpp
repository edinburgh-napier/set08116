#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;

bool load_content() {
  geom.set_type(GL_TRIANGLE_STRIP);
  // Positions
  vector<vec3> positions{vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
                         vec3(1.0f, 1.0f, 0.0f)};
  // Colours
  vector<vec4> colours{vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f),
                       vec4(1.0f, 0.0f, 0.0f, 1.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  // *********************************
  // Load in shaders



  // *********************************

  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // *********************************
  // Set MVP matrix uniform

  // Set offset to (5.0f, 0.0f, 0.0f)

  // *********************************

  // Render geometry
  renderer::render(geom);
  return true;
}

void main() {
  // Create application
  app application("62_Geometry_Shader");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}