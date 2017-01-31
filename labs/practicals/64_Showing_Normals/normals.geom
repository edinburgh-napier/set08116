#version 440

// MVP transformation
uniform mat4 MVP;

// Layout of incoming data
layout(triangles) in;
// Layout of outgoing data
layout(line_strip, max_vertices = 6) out;

// Incoming normals for vertexs
layout(location = 0) flat in vec3 normal[];

void main() {
  // Calculate for each vertex
  for (int i = 0; i < 3; ++i) {
    // *********************************
    // Get vertex pos

    // Ensure normal is normalized


    // Output normal position for start of line
    // - remember to transform

    // Emit

    // Output position + normal for end of line
    // - remember to transform

    // Emit

    // End the primitive

    // *********************************
  }
}