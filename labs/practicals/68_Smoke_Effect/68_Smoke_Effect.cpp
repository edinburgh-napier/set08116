#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace graphics_framework;
using namespace glm;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 4096;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];

GLuint G_Position_buffer, G_Velocity_buffer;

effect eff;
effect compute_eff;
arc_ball_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;
GLuint vao;
texture tex;

bool load_content() {
  default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  uniform_real_distribution<float> dist;

  tex = texture("textures/smoke.png");

  // Initilise particles
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    positions[i] = vec4(((2.0f * dist(rand)) - 1.0f) / 10.0f, 5.0 * dist(rand), 0.0f, 0.0f);
    velocitys[i] = vec4(0.0f, 0.1f + dist(rand), 0.0f, 0.0f);
  }
  // Load in shaders
  eff.add_shader("68_Smoke_Effect/smoke.vert", GL_VERTEX_SHADER);
  eff.add_shader("68_Smoke_Effect/smoke.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("68_Smoke_Effect/smoke.geom", GL_GEOMETRY_SHADER);

  eff.build();

  // Load in shaders
  compute_eff.add_shader("67_Compute_Shader/particle.comp", GL_COMPUTE_SHADER);
  compute_eff.build();

  // a useless vao, but we need it bound or we get errors.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // *********************************
   //Generate Position Data buffer

  // Bind as GL_SHADER_STORAGE_BUFFER

  // Send Data to GPU, use GL_DYNAMIC_DRAW


  // Generate Velocity Data buffer

  // Bind as GL_SHADER_STORAGE_BUFFER

  // Send Data to GPU, use GL_DYNAMIC_DRAW

  // *********************************
   //Unbind
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  // Set camera properties
  cam.set_position(vec3(0.0f, 2.5f, 10.0f));
  cam.set_target(vec3(0.0f, 0, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  if (delta_time > 10.0f) {
    delta_time = 10.0f;
  }
  renderer::bind(compute_eff);
  glUniform3fv(compute_eff.get_uniform_location("max_dims"), 1, &(vec3(3.0f, 5.0f, 5.0f))[0]);
  glUniform1f(compute_eff.get_uniform_location("delta_time"), delta_time);

  // Update the camera
  double current_x;
  double current_y;
  static const float sh = static_cast<float>(renderer::get_screen_height());
  static const float sw = static_cast<float>(renderer::get_screen_height());
  static const double ratio_width = quarter_pi<float>() / sw;
  static const double ratio_height = (quarter_pi<float>() * (sh / sw)) / sh;

  // Get the current cursor position
  glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

  // Calculate delta of cursor positions from last frame
  double delta_x = current_x - cursor_x;
  double delta_y = current_y - cursor_y;

  // Multiply deltas by ratios and delta_time - gets actual change in orientation
  delta_x *= ratio_width;
  delta_y *= ratio_height;

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
  // Bind Compute Shader
  renderer::bind(compute_eff);
  // Bind data as SSBO
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
  // Dispatch
  glDispatchCompute(MAX_PARTICLES / 128, 1, 1);
  // Sync, wait for completion
  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  // *********************************
  // Bind render effect

  // Create MV matrix



  // Set the colour uniform

  // Set MV, and P matrix uniforms seperatly


  // Set point_size size uniform to .1f

  // Bind particle texture


  // *********************************

  // Bind position buffer as GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
  // Setup vertex format
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
  // Enable Blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Disable Depth Mask
  glDepthMask(GL_FALSE);
  // Render
  glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
  // Tidy up, enable depth mask
  glDepthMask(GL_TRUE);
  // Disable Blend
  glDisable(GL_BLEND);
  // Unbind all arrays
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
  return true;
}

void main() {
  // Create application
  app application("68_Smoke_Effect");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}