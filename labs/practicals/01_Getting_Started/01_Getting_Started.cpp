// The main header for the graphics framework
#include <graphics_framework.h>

// The namespaces we are using
using namespace std;
using namespace graphics_framework;

// Initial one-time startup code goes here
bool load_content() { return true; }

// Called every frame, do game logic here
bool update(float delta_time) {
  cout << "FPS: " << 1.0f / delta_time << endl;
  return true;
}

// Called every frame, do rendering here
bool render() { return true; }

void main() {
  // Create application
  app application;
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}