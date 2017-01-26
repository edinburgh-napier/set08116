#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
target_camera cam;
float theta = 0.0f;
float rho = 0.0f;

const int subdivisions = 5;

void divide_triangle(const vector<vec3> &points, int divisions, vector<vec3> &positions, vector<vec4> &colours) {
  // IF we have more divisions to do?
  if (divisions > 0) {
    // *********************************
    // Calculate new vertices to work on (Normalize each element!)


    // Divide new triangles




    // *********************************
  } else {
    positions.insert(positions.end(), points.begin(), points.end());
    for (auto i = 0; i < 3; ++i) {
      colours.push_back(vec4(0.6f, i % 2, i % 3, 1.0f));
    }
  }
}

bool load_content() {
  // Required buffers
  vector<vec3> positions;
  vector<vec4> colours;
  // Define the initial tetrahedron - 4 points
  vector<vec3> v{vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.942809f, -0.333333f), vec3(-0.816497f, -0.471405f, -0.333333f),
                 vec3(0.816497f, -0.471405f, 0.333333f)};
  // Divide the triangles
  divide_triangle({v[0], v[1], v[2]}, subdivisions, positions, colours);
  divide_triangle({v[3], v[2], v[1]}, subdivisions, positions, colours);
  divide_triangle({v[0], v[3], v[1]}, subdivisions, positions, colours);
  divide_triangle({v[0], v[2], v[3]}, subdivisions, positions, colours);

  // Use Line mode to see what this looks like in wireframe, Hint: It's Wack.
  // geom.set_type(GL_LINES);

  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(colours, BUFFER_INDEXES::COLOUR_BUFFER);

  // Load in shaders
  eff.add_shader("shaders/basic.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(3.0f, 3.0f, 3.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    theta -= pi<float>() * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    theta += pi<float>() * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
    rho -= pi<float>() * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
    rho += pi<float>() * delta_time;
  }
  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M = eulerAngleXZ(theta, rho);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Render geometry
  renderer::render(geom);
  return true;
}

void main() {
  // Create application
  app application("23_Sphere_Subdivision");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}