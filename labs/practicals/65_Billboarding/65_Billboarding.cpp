#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
texture tex;
arc_ball_camera cam;

double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {
  glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

  return true;
}

bool load_content() {
  // Vector we will use to store randomly generated points
  vector<vec3> positions;

  // Allows creation of random points.  Note range
  default_random_engine e;
  uniform_real_distribution<float> dist(-100, 100);

  // Randomly generate points
  for (auto i = 0; i < 1000; ++i)
    positions.push_back(vec3(dist(e), dist(e), dist(e)));

  // Create geometry using these points
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  // Set geometry type to points
  geom.set_type(GL_POINTS);

  // Texture we use to texture the billboards
  tex = texture("textures/smiley.png");

  // Load shader
  eff.add_shader("65_Billboarding/shader.vert", GL_VERTEX_SHADER);
  eff.add_shader("65_Billboarding/billboard.geom", GL_GEOMETRY_SHADER);
  eff.add_shader("65_Billboarding/shader.frag", GL_FRAGMENT_SHADER);
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // The ratio of pixels to rotation - remember the fov
  static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
  static double ratio_height =
      (quarter_pi<float>() * renderer::get_screen_aspect()) / static_cast<float>(renderer::get_screen_height());

  double current_x;
  double current_y;
  // Get the current cursor position
  glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

  // Calculate delta of cursor positions from last frame
  double delta_x = current_x - cursor_x;
  double delta_y = current_y - cursor_y;

  // Multiply deltas by ratios and delta_time - gets actual change in orientation
  delta_x *= -ratio_width;
  delta_y *= -ratio_height;

  // Rotate cameras by delta
  cam.rotate(delta_y, delta_x);

  // Use UP and DOWN to change camera distance
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    cam.move(-5.0f * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    cam.move(5.0f * delta_time);
  }

  // Update the camera
  cam.update(delta_time);

  // Update cursor pos
  cursor_x = current_x;
  cursor_y = current_y;

  return true;
}

bool render() {
  // Simply render the points.  All the work done in the geometry shader
  renderer::bind(eff);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V;
  glUniformMatrix4fv(eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(V));
  glUniformMatrix4fv(eff.get_uniform_location("P"), 1, GL_FALSE, value_ptr(P));
  glUniform1f(eff.get_uniform_location("point_size"), 2.0f);
  renderer::bind(tex, 0);
  glUniform1i(eff.get_uniform_location("tex"), 0);

  renderer::render(geom);

  return true;
}

void main() {
  // Create application
  app application("65_Billboarding");
  // Set initialisation, load content, update and render methods
  application.set_initialise(initialise);
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}