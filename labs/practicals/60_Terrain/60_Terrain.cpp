#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh terr;
effect eff;
free_camera cam;
directional_light light;
texture tex[4];

void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth,
                      float height_scale) {
  // Contains our position data
  vector<vec3> positions;
  // Contains our normal data
  vector<vec3> normals;
  // Contains our texture coordinate data
  vector<vec2> tex_coords;
  // Contains our texture weights
  vector<vec4> tex_weights;
  // Contains our index data
  vector<unsigned int> indices;

  // Extract the texture data from the image
  glBindTexture(GL_TEXTURE_2D, height_map.get_id());
  auto data = new vec4[height_map.get_width() * height_map.get_height()];
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void *)data);

  // Determine ratio of height map to geometry
  float width_point = static_cast<float>(width) / static_cast<float>(height_map.get_width());
  float depth_point = static_cast<float>(depth) / static_cast<float>(height_map.get_height());

  // Point to work on
  vec3 point;

  // Part 1 - Iterate through each point, calculate vertex and add to vector
  for (int x = 0; x < height_map.get_width(); ++x) {
    // Calculate x position of point
    point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

    for (int z = 0; z < height_map.get_height(); ++z) {
      // *********************************
      // Calculate z position of point

      // *********************************
      // Y position based on red component of height map data
      point.y = data[(z * height_map.get_width()) + x].y * height_scale;
      // Add point to position data
      positions.push_back(point);
    }
  }

  // Part 1 - Add index data
  for (unsigned int x = 0; x < height_map.get_width() - 1; ++x) {
    for (unsigned int y = 0; y < height_map.get_height() - 1; ++y) {
      // Get four corners of patch
      unsigned int top_left = (y * height_map.get_width()) + x;
      unsigned int top_right = (y * height_map.get_width()) + x + 1;
      // *********************************


      // *********************************
      // Push back indices for triangle 1 (tl,br,bl)
      indices.push_back(top_left);
      indices.push_back(bottom_right);
      indices.push_back(bottom_left);
      // Push back indices for triangle 2 (tl,tr,br)
      // *********************************



      // *********************************
    }
  }

  // Resize the normals buffer
  normals.resize(positions.size());

  // Part 2 - Calculate normals for the height map
  for (unsigned int i = 0; i < indices.size() / 3; ++i) {
    // Get indices for the triangle
    auto idx1 = indices[i * 3];
    auto idx2 = indices[i * 3 + 1];
    auto idx3 = indices[i * 3 + 2];

    // Calculate two sides of the triangle
    vec3 side1 = positions[idx1] - positions[idx3];
    vec3 side2 = positions[idx1] - positions[idx2];

    // Normal is normal(cross product) of these two sides
    // *********************************


    // Add to normals in the normal buffer using the indices for the triangle



    // *********************************
  }

  // Normalize all the normals
  for (auto &n : normals) {
    // *********************************

    // *********************************
  }

  // Part 3 - Add texture coordinates for geometry
  for (unsigned int x = 0; x < height_map.get_width(); ++x) {
    for (unsigned int z = 0; z < height_map.get_height(); ++z) {
      tex_coords.push_back(vec2(width_point * x, depth_point * z));
    }
  }

  // Part 4 - Calculate texture weights for each vertex
  for (unsigned int x = 0; x < height_map.get_width(); ++x) {
    for (unsigned int z = 0; z < height_map.get_height(); ++z) {
      // Calculate tex weight
      vec4 tex_weight(clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
                      clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
                      clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
                      clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));

      // *********************************
      // Sum the components of the vector

      // Divide weight by sum

      // Add tex weight to weights

      // *********************************
    }
  }

  // Add necessary buffers to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
  geom.add_index_buffer(indices);

  // Delete data
  delete[] data;
}

bool load_content() {
  // Geometry to load into
  geometry geom;

  // Load height map
  texture height_map("textures/heightmap.jpg");

  // Generate terrain
  generate_terrain(geom, height_map, 20, 20, 2.0f);

  // Use geometry to create terrain mesh
  terr = mesh(geom);

  // Load in necessary shaders
  eff.add_shader("60_Terrain/terrain.vert", GL_VERTEX_SHADER);
  eff.add_shader("60_Terrain/terrain.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  eff.add_shader("60_Terrain/part_weighted_texture_4.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Material definitions
  light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
  light.set_light_colour(vec4(0.9f, 0.9f, 0.9f, 1.0f));
  light.set_direction(normalize(vec3(1.0f, 1.0f, 1.0f)));
  terr.get_material().set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
  terr.get_material().set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  terr.get_material().set_shininess(20.0f);
  terr.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));

  // terrian trextures
  tex[0] = texture("textures/sand.jpg");
  tex[1] = texture("textures/grass.jpg");
  tex[2] = texture("textures/stone.jpg");
  tex[3] = texture("textures/snow.jpg");

  // Set camera properties
  cam.set_position(vec3(0.0f, 5.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return true;
}

bool update(float delta_time) {
  // The ratio of pixels to rotation - remember the fov
  static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
  static double ratio_height =
      (quarter_pi<float>() * renderer::get_screen_aspect()) / static_cast<float>(renderer::get_screen_height());
  static double cursor_x = 0.0;
  static double cursor_y = 0.0;
  double current_x;
  double current_y;
  // Get the current cursor position
  glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
  // Calculate delta of cursor positions from last frame
  double delta_x = current_x - cursor_x;
  double delta_y = current_y - cursor_y;
  // Multiply deltas by ratios - gets actual change in orientation
  delta_x *= ratio_width;
  delta_y *= ratio_height;
  // Rotate cameras by delta
  cam.rotate(delta_x, -delta_y);
  // Use keyboard to move the camera - WASD
  vec3 translation(0.0f, 0.0f, 0.0f);
  if (glfwGetKey(renderer::get_window(), 'W')) {
    translation.z += 5.0f * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), 'S')) {
    translation.z -= 5.0f * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), 'A')) {
    translation.x -= 5.0f * delta_time;
  }
  if (glfwGetKey(renderer::get_window(), 'D')) {
    translation.x += 5.0f * delta_time;
  }
  // Move camera
  cam.move(translation);
  // Update the camera
  cam.update(delta_time);
  // Update cursor pos
  cursor_x = current_x;
  cursor_y = current_y;
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);
  // Create MVP matrix
  auto M = terr.get_transform().get_transform_matrix();
  auto V = cam.get_view();
  auto P = cam.get_projection();
  auto MVP = P * V * M;
  // Set MVP matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  // Set M matrix uniform
  glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
  // Set N matrix uniform
  glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(terr.get_transform().get_normal_matrix()));
  // *********************************
  // Set eye_pos uniform to camera position

  // *********************************
   //Bind Terrian Material
  renderer::bind(terr.get_material(), "mat");
  // Bind Light
  renderer::bind(light, "light");
  // Bind Tex[0] to TU 0, set uniform
  renderer::bind(tex[0], 0);
  glUniform1i(eff.get_uniform_location("tex[0]"), 0);
  // *********************************
   //Bind Tex[1] to TU 1, set uniform


  // Bind Tex[2] to TU 2, set uniform


  // Bind Tex[3] to TU 3, set uniform


  // *********************************
  // Render terrain
  renderer::render(terr);

  return true;
}

void main() {
  // Create application
  app application("60_Terrain");
  // Set methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}