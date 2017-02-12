#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace graphics_framework;
using namespace glm;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 2 << 11;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];

GLuint G_Position_buffer, G_Velocity_buffer;

effect eff;
effect compute_eff;
target_camera cam;
GLuint vao;

bool load_content() {
  cout << "Generating " << MAX_PARTICLES << " Particles" << endl;
  default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  uniform_real_distribution<float> dist;

  // Initilise particles
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    positions[i] = vec4(((14.0f * dist(rand)) - 7.0f), 8.0f * dist(rand), 0.0f, 0.0f);
    velocitys[i] = vec4(0.0f, 0.1f + (2.0f * dist(rand)), 0.0f, 0.0f);
  }

  // Load in shaders
  eff.add_shader("shaders/basic_colour.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic_colour.frag", GL_FRAGMENT_SHADER);
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

  renderer::setClearColour(0, 0, 0);

  // Set camera properties
  cam.set_position(vec3(0, 4.0f, 10.0f));
  cam.set_target(vec3(0.0f, 4.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  renderer::bind(compute_eff);
  glUniform1f(compute_eff.get_uniform_location("delta_time"), delta_time);
  glUniform3fv(compute_eff.get_uniform_location("max_dims"), 1, value_ptr(vec3(7.0f, 8.0f, 5.0f)));

  // Update the camera
  cam.update(delta_time);
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

  // Bind render effect
  renderer::bind(eff);
  // Create MVP matrix
  mat4 M(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set the colour uniform
  glUniform4fv(eff.get_uniform_location("colour"), 1, value_ptr(vec4(1.0f)));
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

  // Bind position buffer as GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
  // Setup vertex format
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
  // Render
  glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
  // Tidy up
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);

  return true;
}

void main() {
  // Create application
  app application("67_Compute_Shader");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}