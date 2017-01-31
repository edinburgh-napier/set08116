#include <graphics_framework.h>

using namespace std;
using namespace std::chrono;
using namespace glm;
using namespace graphics_framework;

// Maximum number of particles
const unsigned int MAX_PARTICLES = 1000;

// A particle
struct particle
{
    vec3 position = vec3(0, 0, 0);
    vec3 velocity = vec3(0, 0, 0);
};

// Particles in the system
particle particles[MAX_PARTICLES];

// The transform feedback buffers
GLuint transform_feedbacks[2];
// The particle buffers
GLuint particle_buffers[2];

// Effects
effect eff;
effect particle_eff;

// Current buffer to perform the physics update to
unsigned int front_buf = 0;
// Buffer to render to
unsigned int back_buf = 1;

arc_ball_camera cam;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise()
{
    glPointSize(5.0f);
    glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

    return true;
}

bool load_content()
{
    default_random_engine rand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    uniform_real_distribution<float> dist;

    // ******************************************
    // Generate transform feedback buffers on GPU
    // ******************************************
    glGenTransformFeedbacks(2, transform_feedbacks);
    // ********************************
    // Generate particle buffers on GPU
    // ********************************
    glGenBuffers(2, particle_buffers);

    // *******************
    // Initilise particles
    // *******************
    for (unsigned int i = 0; i < MAX_PARTICLES; ++i)
    {
        particles[i].position = vec3(((2.0f * dist(rand)) - 1.0f) / 10.0f, 0.0f, 1.0f);
        particles[i].velocity = vec3(0.0f, 0.1f + dist(rand), 0.0f);
    }

    // ************************
    // Allocate the two buffers
    // ************************
    for (unsigned int i = 0; i < 2; ++i)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particle) * MAX_PARTICLES, particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particle_buffers[i]);
    }

    // Build effects
    eff.add_shader("..\\resources\\shaders\\colour.vert", GL_VERTEX_SHADER);
    eff.add_shader("..\\resources\\shaders\\colour.frag", GL_FRAGMENT_SHADER);
    eff.build();

    particle_eff.add_shader("particle.vert", GL_VERTEX_SHADER);
    particle_eff.add_shader("particle.geom", GL_GEOMETRY_SHADER);
    particle_eff.add_shader("particle.frag", GL_FRAGMENT_SHADER);
    particle_eff.build();

    // Use the particle effect
    renderer::bind(particle_eff);

    // **************************************
    // Tell OpenGL what the output looks like
    // **************************************
    const GLchar* attrib_names[2] =
    {
        "position_out",
        "velocity_out"
    };
    glTransformFeedbackVaryings(particle_eff.get_program(), 2, attrib_names, GL_INTERLEAVED_ATTRIBS);

    // **************
    // Relink program
    // **************
    glLinkProgram(particle_eff.get_program());

    // Set camera properties
    cam.set_position(vec3(0.0f, 00.0f, 10.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    static bool first_frame = true;

    // ********************
    // Bind particle effect
    // ********************
    renderer::bind(particle_eff);
    // **************************
    // Set the delta_time uniform
    // **************************
    glUniform1f(particle_eff.get_uniform_location("delta_time"), delta_time);

    // *******************************
    // Tell OpenGL we aren't rendering
    // *******************************
    glEnable(GL_RASTERIZER_DISCARD);

    // *******************************
    // Bind the buffers for use
    // - buffer is front buf
    // - transform feeback is back buf
    // *******************************
    glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[front_buf]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks[back_buf]);

    // ********************************************
    // Define how our data looks like to the shader
    // ********************************************
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)12);

    // ******************************
    // Perform the transform feedback
    // ******************************
    glBeginTransformFeedback(GL_POINTS);
    // Check if first frame
    if (first_frame)
    {
        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
        first_frame = false;
    }
    // Otherwise perform the draw using front buffer
    else
        glDrawTransformFeedback(GL_POINTS, transform_feedbacks[front_buf]);

    // **************************
    // End the transform feedback
    // **************************
    glEndTransformFeedback();

    // ***********************************
    // Disable the vertex attribute arrays
    // ***********************************
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // *************************
    // Switch on rendering again
    // *************************
    glDisable(GL_RASTERIZER_DISCARD);

    // The ratio of pixels to rotation - remember the fov
    static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
    static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());

    double current_x;
    double current_y;
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
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
        cam.move(-5.0f * delta_time);
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
        cam.move(5.0f * delta_time);

    // Update the camera
    cam.update(delta_time);

    // Update cursor pos
    cursor_x = current_x;
    cursor_y = current_y;
    
    return true;
}

bool render()
{
    // Bind the effect
    renderer::bind(eff);
    // Set the MVP matrix
    auto M = mat4(1.0f);
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    glUniformMatrix4fv(
        eff.get_uniform_location("MVP"),
        1,
        GL_FALSE,
        value_ptr(MVP));
    // Set the colour uniform
    glUniform4fv(eff.get_uniform_location("colour"), 1, value_ptr(vec4(1.0f, 0.0f, 0.0f, 1.0f)));

    // *******************************************
    // Bind the back particle buffer for rendering
    // *******************************************
    glBindBuffer(GL_ARRAY_BUFFER, particle_buffers[back_buf]);

    // ******************************************************
    // Describe the data we are interested in (just position)
    // ******************************************************
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), 0);
    // ****************************************************
    // Perform the render by drawing the transform feedback
    // ****************************************************
    glDrawTransformFeedback(GL_POINTS, transform_feedbacks[back_buf]);
    // ******************************
    // Disable vertex attribute array
    // ******************************
    glDisableVertexAttribArray(0);

    // ***************************
    // Swap front and back buffers
    // ***************************
    front_buf = back_buf;
    back_buf = (back_buf + 1) % 2;

    return true;
}

void main()
{
    // Create application
    app application;
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