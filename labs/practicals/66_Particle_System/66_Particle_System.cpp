#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace glm;
using namespace graphics_framework;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 3000;

// A particle
struct particle {
  vec3 position = vec3(0, 0, 0);
  vec3 velocity = vec3(0, 0, 0);
};

// Particles in the system
particle particles[MAX_PARTICLES];

// The transform feedback buffers
GLuint transform_feedbacks[2];
// The particle buffers
GLuint particle_buffers_vbo[2];

GLuint vao;

// Effects
effect eff;
effect particle_eff;

// Current buffer to perform the physics update to
unsigned int front_buf = 0;
// Buffer to render to
unsigned int back_buf = 1;

target_camera cam;

bool initialise() {
  glPointSize(10.0f);
  glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return true;
}

bool load_content() {
  default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  uniform_real_distribution<float> dist;

  // Initilise particles
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    particles[i].position = vec3(((10.0f * dist(rand)) - 5.0f), 0.0f, 0.0f);
    particles[i].velocity = vec3(0.0f, 0.1f + dist(rand), 0.0f);
  }

  // Build effects
  eff.add_shader("shaders/basic_colour.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/basic_colour.frag", GL_FRAGMENT_SHADER);
  eff.build();

  particle_eff.add_shader("66_Particle_System/particle.vert", GL_VERTEX_SHADER);
  particle_eff.add_shader("66_Particle_System/particle.geom", GL_GEOMETRY_SHADER);
  particle_eff.add_shader("66_Particle_System/particle.frag", GL_FRAGMENT_SHADER);
  particle_eff.build();

  // Use the particle effect
  renderer::bind(particle_eff);
  // Tell OpenGL what the output looks like
  const GLchar *attrib_names[2] = {"position_out", "velocity_out"};
  glTransformFeedbackVaryings(particle_eff.get_program(), 2, attrib_names, GL_INTERLEAVED_ATTRIBS);

  // Relink program
  glLinkProgram(particle_eff.get_program());

  // a useless vao, but we need it bound or we get errors.
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Gen our VBO data buffers
  glGenBuffers(2, particle_buffers_vbo);
  // *********************************
  // Place initial particle data in buffer 1



  // Fill space with blank data in buffer 2



  // generate our feedback objects

  // link fb[0] to vbo[1]


  // *********************************
  // link fb[1] to vbo[0]
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[1]);
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_buffers_vbo[0]);

  renderer::setClearColour(0, 0, 0);

  // Set camera properties
  cam.set_position(vec3(0, 4.0f, 10.0f));
  cam.set_target(vec3(0.0f, 4.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  static bool first_frame = true;
  renderer::bind(particle_eff);
  glUniform1f(particle_eff.get_uniform_location("delta_time"), delta_time);

  // Update the camera
  cam.update(delta_time);
  return true;
}

void render_feeback() {
  static bool first_frame = true;
  renderer::bind(particle_eff);

  // Close down the output
  glEnable(GL_RASTERIZER_DISCARD);

  glBindBuffer(GL_ARRAY_BUFFER, particle_buffers_vbo[front_buf]);
  {
    // unfortunatly we can't save this stuff in the VAO. Because OGL.
    GLint position_in_loc = glGetAttribLocation(particle_eff.get_program(), "position_in");
    GLint velocity_in_loc = glGetAttribLocation(particle_eff.get_program(), "velocity_in");
    glEnableVertexAttribArray(position_in_loc);
    glEnableVertexAttribArray(velocity_in_loc);
    glVertexAttribPointer(position_in_loc, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid *)0);
    glVertexAttribPointer(velocity_in_loc, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid *)12);
  }

  // Tell openGL that we are rendering to feedback
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[front_buf]);
  glBeginTransformFeedback(GL_POINTS);

  if (first_frame) {
    // compute our points from teh VBO like normal
    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
    first_frame = false;
  } else {
    // compute from feedback data
    glDrawTransformFeedback(GL_POINTS, transform_feedbacks[back_buf]);
  }
  // Note: the above doesn't actually render, it saves data into either vbo 0/1.

  // stop feedback recording.
  glEndTransformFeedback();

  // reenable output
  glDisable(GL_RASTERIZER_DISCARD);
}

bool render() {
  render_feeback();

  // Bind the effect
  renderer::bind(eff);
  // Set the MVP matrix
  auto M = mat4(1.0f);
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Set the colour uniform
  glUniform4fv(eff.get_uniform_location("colour"), 1, value_ptr(vec4(1.0f)));

  // Bind the back particle buffer for rendering
  glBindBuffer(GL_ARRAY_BUFFER, particle_buffers_vbo[front_buf]);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), 0);

  // Perform the render by drawing the transform feedback
  glDrawTransformFeedback(GL_POINTS, transform_feedbacks[front_buf]);
  // Disable vertex attribute array
  glDisableVertexAttribArray(0);

  // *********************************
  // Swap front and back buffers


  // *********************************
  return true;
}

void main() {
  // Create application
  app application("66_Particle_System");
  // Set initialise
  application.set_initialise(initialise);
  // Set load content
  application.set_load_content(load_content);
  // Set update
  application.set_update(update);
  // Set render function
  application.set_render(render);
  // Run application
  application.run();
}