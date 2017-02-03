#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace graphics_framework;
using namespace glm;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 2048;

// A particle
struct particle {
  vec3 position = vec3(0, 0, 0);
  vec3 velocity = vec3(0, 0, 0);
};

// Particles in the system
particle particles[MAX_PARTICLES];
vec3 positions[MAX_PARTICLES];
vec3 velocitys[MAX_PARTICLES];

GLuint G_ObjectsBuffer, G_Compute_Buffers, G_Position_buffer, G_Velocity_buffer, G_Attractor_buffer,
    G_AttractorMass_buffer, G_Life_buffer;

effect eff;
effect compute_eff;
target_camera cam;
GLuint vao;
bool load_content() {
  default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  uniform_real_distribution<float> dist;

  // Initilise particles
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    particles[i].position = vec3(((10.0f * dist(rand)) - 5.0f), 1.0f, 0.0f);
    particles[i].velocity = vec3(0.0f, 0.1f + dist(rand), 0.0f);
    positions[i] = particles[i].position;
    velocitys[i] = particles[i].velocity;
  }

  // Load in shaders
  eff.add_shader("shaders/basic_colour.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic_colour.frag", GL_FRAGMENT_SHADER);
  eff.build();
  // Load in shaders
  compute_eff.add_shader("67_Compute_Shader/particle.comp", GL_COMPUTE_SHADER);
  compute_eff.build();

  glGenBuffers(1, &G_Position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(vec3), positions, GL_DYNAMIC_COPY);
  glGenBuffers(1, &G_Velocity_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, G_Velocity_buffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(vec3), velocitys, GL_DYNAMIC_COPY);

  // a useless vao, but we need it bound or we get errors.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

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

  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {

  renderer::bind(compute_eff);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
  glDispatchCompute(MAX_PARTICLES / 128, 1, 1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  // Bind effect
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

  glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

  glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

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