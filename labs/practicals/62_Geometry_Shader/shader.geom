#version 440

// Model view projection matrix
uniform mat4 MVP;

// Offset position
uniform vec3 offset;

// Layout of incoming data
layout(triangles) in;
// Layout of outgoing data
layout(triangle_strip, max_vertices = 9) out;

// Outgoing colour for the vertex
layout(location = 0) out vec4 colour_out;

void main() {

  // Starting triangle is in original position
  for (int i = 0; i < 3; ++i) {
    // Transform vertex into screen space
    gl_Position = MVP * gl_in[i].gl_Position;
    // Starting quad is red
    colour_out = vec4(1.0, 0.0, 0.0, 1.0);
    // Emit the vertex
    EmitVertex();
  }

  // Finished triangle
  EndPrimitive();

  // Emit a copy of the triangle moved by offset
  // Offset triangle needs to be green
  for (int i = 0; i < 3; ++i) {
    gl_Position = MVP * (gl_in[i].gl_Position + vec4(offset, 0));
    // *********************************


    // *********************************
  }
  EndPrimitive();

  // *********************************
  // Emit a copy of the triangle moved by negative offset
  // Offset triangle needs to be blue






  // *********************************
}