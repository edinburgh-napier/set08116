#version 440

// Vertex position
layout (location = 0) in vec3 position;
// Vertex normal
layout (location = 2) in vec3 normal;

// Outgoing normal
layout (location = 0)  out vec3 normal_out;

void main()
{
	// Pass through position to geometry shader
	gl_Position = vec4(position, 1.0);
	// Output normal
	normal_out = normal;
}