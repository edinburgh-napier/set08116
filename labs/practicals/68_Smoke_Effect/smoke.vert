#version 440 core

uniform mat4 MV;

layout(location = 0) in vec3 position;

layout(location = 0) out float height;

void main() {
  gl_Position = MV * vec4(position, 1.0);
  height = position.y;
}