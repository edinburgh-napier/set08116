#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

// Two meshes
array<mesh, 3> meshes;
effect eff;
target_camera cam;
// Two textures
array<texture, 3> texs;

bool load_content() {
  // Construct geometry object
  geometry geom;
  geom.set_type(GL_TRIANGLE_STRIP);
  // Create quad data
  // Positions
  vector<vec3> positions{vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
                         vec3(1.0f, 1.0f, 0.0f)};
  // Texture coordinates
  vector<vec2> tex_coords{vec2(0.0f, 0.0f), vec2(10.0f, 0.0f), vec2(0.0f, 10.0f), vec2(10.0f, 10.0f)};
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Create three mesh objects - reuse geometry
  for (auto &m : meshes) {
    // *********************************

    // Scale each mesh by 10

    // *********************************
  }

  // Set mesh positions - remember scale
  for (unsigned int i = 0; i < meshes.size(); ++i) {
    meshes[i].get_transform().translate((static_cast<float>(i) * vec3(21.0f, 0.0f, 0.0f)) - vec3(21.0f, 0.0f, 0));
  }
  // Load in texture shaders, !Note that are pulling in shader file from previous project!
  eff.add_shader("31_Texturing_Shader/simple_texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("31_Texturing_Shader/simple_texture.frag", GL_FRAGMENT_SHADER);

  // Build effect
  eff.build();

  // Load textures
  // 0 - no mipmaps
  texs[0] = texture("textures/checker.png", false, false);
  // 1 - Auto Generate mipmaps
  texs[1] = texture("textures/checker.png", true, false);
  // 2 - Manual Mip Levels
  texs[2] = texture({"textures/uv_32.png", "textures/uv_16.png", "textures/uv_8.png", "textures/uv_4.png",
                     "textures/uv_2.png", "textures/uv_1.png"},
                    false);

  // Set camera properties
  cam.set_position(vec3(0, 0, 10.0f));
  cam.set_target(vec3(0, 0, 1.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 0.0001f, 10000.0f);

  return true;
}

bool update(float delta_time) {
  // Move camera with cursor
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
    cam.set_position(cam.get_position() - vec3(0.0f, 0.0f, 10.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
    cam.set_position(cam.get_position() + vec3(0.0f, 0.0f, 10.0f) * delta_time);
  }
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

    // *********************************
    // Bind correct texture to renderer

    // Set the texture value for the shader here

    // Render the mesh

    // *********************************
  }

  return true;
}

void main() {
  // Create application
  app application("30_Mipmaps");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}