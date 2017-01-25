#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

// Four meshes
array<mesh, 4> meshes;
effect eff;
free_camera cam;
// Four textures
array<texture, 4> texs;

bool load_content() {
  // Construct geometry object
  geometry geom;
  geom.set_type(GL_TRIANGLE_STRIP);
  // Positions
  vector<vec3> positions{vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
                         vec3(1.0f, 1.0f, 0.0f)};
  // Texture coordinates
  vector<vec2> tex_coords{vec2(0.0f, 0.0f), vec2(10.0f, 0.0f), vec2(0.0f, 400.0f), vec2(10.0f, 400.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Create  mesh objects - reuse geometry
  for (unsigned int i = 0; i < meshes.size(); ++i) {
    meshes[i] = mesh(geom);
    meshes[i].get_transform().scale = vec3(10.0f, 400.0f, 10.0f);
    meshes[i].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, 0.0f));
    meshes[i].get_transform().translate((static_cast<float>(i) * vec3(21.0f, 0.0f, 0.0f)) - vec3(21.0f, 0.0f, 0));
  }

  // Load in texture shaders, !Note that are pulling in shader file from previous project!
  eff.add_shader("31_Texturing_Shader/simple_texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("31_Texturing_Shader/simple_texture.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // ******************************
  // Load textures sign.jpg
  // 0 - no mipmaps, no anisotropic
  // 1 - no mipmaps, anisotropic
  // 2 - mipmaps, no anisotropic
  // 3 - mipmaps, anisotropic
  // ******************************
  texs[0] = texture("textures/checker.png", false, false);
  texs[1] = texture("textures/checker.png", false, true);
  texs[2] = texture("textures/checker.png", true, true);
  texs[3] = texture("textures/checker.png", true, true);

  // Set camera properties
  cam.set_position(vec3(10.0f, 2.0f, 200.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

  return true;
}

bool update(float delta_time) {
  // Move camera, notice the slightly altered technique
  vec3 dir;
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    dir += vec3(0.0f, 10.0f, 0.0f);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    dir += vec3(0.0f, -10.0f, 0.0f);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
    dir += vec3(-10.0f, 0.0f, 0.0f);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
    dir += vec3(10.0f, 0.0f, 0.0f);
  }
  cam.set_position(cam.get_position() + dir * delta_time);
  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Render each mesh
  for (unsigned int i = 0; i < meshes.size(); ++i) {
    // Create MVP matrix
    auto M = meshes[i].get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

    // Bind correct texture to renderer
    renderer::bind(texs[i], 0);

    // Set the texture value for the shader here
    glUniform1i(eff.get_uniform_location("tex"), 0);

    // Render the mesh
    renderer::render(meshes[i]);
  }

  return true;
}

void main() {
  // Create application
  app application("31_Anisotropic");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}