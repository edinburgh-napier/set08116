#include <graphics_framework.h>
#include <glm\glm.hpp>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh sphere;
effect eff;
target_camera cam;
float explode_factor = 0.0f;

bool load_content()
{
    // Create a cylinder
    sphere = mesh(geometry_builder::create_sphere(100, 100));
    // Scale cylinder
    sphere.get_transform().scale = vec3(5.0f, 5.0f, 5.0f);

    // Load in shaders
    eff.add_shader("shader.vert", GL_VERTEX_SHADER);
    eff.add_shader("shader.frag", GL_FRAGMENT_SHADER);
    eff.add_shader("..\\resources\\shaders\\explode.geom", GL_GEOMETRY_SHADER);

    // Build effect
    eff.build();

    // Set camera properties
    cam.set_position(vec3(0.0f, 2.0f, 50.0f));
    cam.set_target(vec3(0.0f, 0.0f, 0.0f));
    auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
    cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
    return true;
}

bool update(float delta_time)
{
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT))
        sphere.get_transform().rotate(vec3(0.0f, -pi<float>(), 0.0f) * delta_time);
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT))
        sphere.get_transform().rotate(vec3(0.0f, pi<float>(), 0.0f) * delta_time);
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
        sphere.get_transform().rotate(vec3(pi<float>(), 0.0f, 0.0f) * delta_time);
    if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
        sphere.get_transform().rotate(vec3(-pi<float>(), 0.0f, 0.0f) * delta_time);

    // ************************************
    // Use o and p to modify explode factor
    // ************************************
    if (glfwGetKey(renderer::get_window(), 'O'))
        explode_factor += 0.1f;
    if (glfwGetKey(renderer::get_window(), 'P'))
        explode_factor -= 0.1f;

    cam.update(delta_time);

    return true;
}

bool render()
{
    // Bind effect
    renderer::bind(eff);
    // Create MVP matrix
    auto M = sphere.get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(
        eff.get_uniform_location("MVP"),
        1,
        GL_FALSE,
        value_ptr(MVP));
    // Set N matrix uniform
    glUniformMatrix3fv(
        eff.get_uniform_location("N"),
        1,
        GL_FALSE,
        value_ptr(sphere.get_transform().get_normal_matrix()));

    // ******************
    // Set explode factor
    // ******************
    glUniform1f(eff.get_uniform_location("explode_factor"), explode_factor);

    // Render mesh
    renderer::render(sphere);

    return true;
}

void main()
{
    // Create application
    app application;
    // Set load content, update and render methods
    application.set_load_content(load_content);
    application.set_update(update);
    application.set_render(render);
    // Run application
    application.run();
}