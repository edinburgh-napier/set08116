#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect main_eff;
effect shadow_eff;
texture tex;
target_camera cam;
spot_light spot;
shadow_map shadow;

bool load_content() {
  // *********************************
  // Create shadow map- use screen size

  // Create plane mesh

  // Create "teapot" mesh by loading in models/teapot.obj

  // Translate Teapot(0,4,0)

  // Scale the teapot - (0.1, 0.1, 0.1)

  // *********************************

  // Load texture
  tex = texture("textures/checked.gif");

  // ***********************
  // Set materials
  // - all emissive is black
  // - all specular is white
  // - all shininess is 25
  // ***********************
  // White plane
  meshes["plane"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["plane"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["plane"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["plane"].get_material().set_shininess(25.0f);
  // Red teapot
  meshes["teapot"].get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  meshes["teapot"].get_material().set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  meshes["teapot"].get_material().set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["teapot"].get_material().set_shininess(25.0f);

  // *******************
  // Set spot properties
  // *******************
  // Pos (20, 30, 0), White
  // Direction (-1, -1, 0) normalized
  // 50 range, 10 power
  spot.set_position(vec3(30.0f, 20.0f, 0.0f));
  spot.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  spot.set_direction(normalize(-spot.get_position()));
  spot.set_range(500.0f);
  spot.set_power(10.0f);

  // Load in shaders
  main_eff.add_shader("54_Shadowing/shadow.vert", GL_VERTEX_SHADER);
  vector<string> frag_shaders{"54_Shadowing/shadow.frag", "shaders/part_spot.frag", "shaders/part_shadow.frag"};
  main_eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  shadow_eff.add_shader("50_Spot_Light/spot.vert", GL_VERTEX_SHADER);
  shadow_eff.add_shader("50_Spot_Light/spot.frag", GL_FRAGMENT_SHADER);

  // Build effects
  main_eff.build();
  shadow_eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 50.0f, -75.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Rotate the teapot
  meshes["teapot"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

  if (glfwGetKey(renderer::get_window(), '1')) {
    cam.set_position(vec3(0.0f, 50.0f, -75.0f));
  }
  if (glfwGetKey(renderer::get_window(), '2')) {
    cam.set_position(spot.get_position());
  }
  if (glfwGetKey(renderer::get_window(), '3')) {
    cam.set_position(vec3(-25.0, 50.0, 0.0));
  }
  if (glfwGetKey(renderer::get_window(), '4')) {
    cam.set_position(vec3(-50, 2.0, 0));
  }
  cam.update(delta_time);

  // *********************************
  // Update the shadow map light_position from the spot light

  // do the same for light_dir property

  // *********************************

  // Press s to save
  if (glfwGetKey(renderer::get_window(), 'S') == GLFW_PRESS)
    shadow.buffer->save("test.png");

  return true;
}

bool render() {
  // *********************************
  // Set render target to shadow map

  // Clear depth buffer bit

  // Set face cull mode to front

  // *********************************

  // We could just use the Camera's projection, 
  // but that has a narrower FoV than the cone of the spot light, so we would get clipping.
  // so we have yo create a new Proj Mat with a field of view of 90.
  mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

  // Bind shader
  renderer::bind(shadow_eff);
  // Render meshes
  for (auto &e : meshes) {
    auto m = e.second;
    // Create MVP matrix
    auto M = m.get_transform().get_transform_matrix();
    // *********************************
    // View matrix taken from shadow map

    // *********************************
    auto MVP = LightProjectionMat * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(shadow_eff.get_uniform_location("MVP"), // Location of uniform
                       1,                                      // Number of values - 1 mat4
                       GL_FALSE,                               // Transpose the matrix?
                       value_ptr(MVP));                        // Pointer to matrix data
    // Render mesh
    renderer::render(m);
  }
  // *********************************
  // Set render target back to the screen

  // Set face cull mode to back

  // *********************************

  // Bind shader
  renderer::bind(main_eff);

  // Render meshes
  for (auto &e : meshes) {
    auto m = e.second;
    // Create MVP matrix
    auto M = m.get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(main_eff.get_uniform_location("MVP"), // Location of uniform
                       1,                                    // Number of values - 1 mat4
                       GL_FALSE,                             // Transpose the matrix?
                       value_ptr(MVP));                      // Pointer to matrix data
    // Set M matrix uniform
    glUniformMatrix4fv(main_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
    // Set N matrix uniform
    glUniformMatrix3fv(main_eff.get_uniform_location("N"), 1, GL_FALSE,
                       value_ptr(m.get_transform().get_normal_matrix()));
    // *********************************
    // Set lightMVP uniform, using:
     //Model matrix from m

    // viewmatrix from the shadow map

    // Multiply together with LightProjectionMat

    // Set uniform

    // Bind material

    // Bind spot light

    // Bind texture

    // Set tex uniform

    // Set eye position

    // Bind shadow map texture - use texture unit 1

    // Set the shadow_map uniform

    // Render mesh

    // *********************************
  }

  return true;
}

void main() {
  // Create application
  app application("54_Shadowing");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}