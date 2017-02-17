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

  // Need to rotate the teapot on x by negative pi/2

  // Scale the teapot - (0.1, 0.1, 0.1)

  // *********************************

  // Load texture
  tex = texture("textures/checker.png");

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
  meshes["teapot"].get_material().set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
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
  spot.set_direction(normalize(vec3(-1.0f, -1.0f, 0.0f)));
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
  meshes["teapot"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()) * delta_time);

  // *********************************
  // Update the shadow map properties from the spot light


  // *********************************

  // Press s to save
  if (glfwGetKey(renderer::get_window(), 'S') == GLFW_PRESS)
    shadow.buffer->save("test.png");

  cam.update(delta_time);

  return true;
}

bool render() {
  // *********************************
  // Set render target to shadow map

  // Clear depth buffer bit

  // Set render mode to cull face

  // *********************************

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

    auto P = cam.get_projection();
    auto MVP = P * V * M;
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

  // Set cull face to back

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
    // Set light transform





    // Bind material

    // Bind spot lights

    // Bind texture

    // Set tex uniform

    // Set eye position

    // Bind shadow map texture - use texture unit 1

	//Set the shadow_map uniform

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