#version 440

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	// Normals are green
	colour = vec4(0.0, 1.0, 0.0, 1.0);
}