#version 440 core

uniform mat4 P;
uniform float point_size;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) in float height[];

// Let's use this syntax for a change
out VertexData {
  vec4 colour;
  vec2 tex_coord;
};

void main() {
  vec4 position = gl_in[0].gl_Position;

  // fire temperature
  float temp = clamp(2.0 / (2.0 / (height[0])), 0.0, 1.0);
  // scale between white and red
  vec4 fire_colour = mix(vec4(1., .98, .42, 1.), vec4(0.88, .35, 0., 1.), temp);
  // and then between red and black
  fire_colour = mix(fire_colour, vec4(0), height[0] - 1.0);
  // fade smoke out near top
  fire_colour.a = clamp((2.0 - height[0]) / 3.0, 0.0, 1.0);

  // Expand point to 4 verts
   //point VA (-0.5, -0.5)
  vec2 va = position.xy + vec2(-0.5, -0.5) * point_size;
  gl_Position = P * vec4(va, position.zw);
  tex_coord = vec2(0.0, 0.0);
  colour = fire_colour;
  EmitVertex();
  // *********************************
   //point VB (0.5, -0.5)





  // point VD (-0.5, -0.5)





  // point VC (0.5, -0.5)





  // *********************************

  EndPrimitive();
}