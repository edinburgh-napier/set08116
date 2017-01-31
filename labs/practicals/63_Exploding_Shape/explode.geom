#version 440

// Scale for normal
uniform float explode_factor;

// Layout of incoming data
layout(triangles) in;
// Layout of outgoing data
layout(triangle_strip, max_vertices = 3) out;

// Outgoing colour for the vertex
layout(location = 0) out vec4 colour_out;

void main() {

  // Calculate Face Normal
  vec3 P0 = gl_in[0].gl_Position.xyz;
  vec3 P1 = gl_in[1].gl_Position.xyz;
  vec3 P2 = gl_in[2].gl_Position.xyz;
  vec3 V0 = P0 - P1;
  vec3 V1 = P2 - P1;
  vec3 face_normal = normalize(cross(V1, V0));

  for (int i = 0; i < 3; i++) {
    // *********************************
    // Change triangle position += vec4(face_normal, 0.0) * explode_factor

    //Colour the vertex with the normal

    // Emit Vertex

    // *********************************
  }
  // End Primitive
  EndPrimitive();
}