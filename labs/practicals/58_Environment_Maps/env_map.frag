#version 440

// Cubemap data
uniform samplerCube cubemap;

// Incoming 3D tex_coord
layout (location = 0) in vec3 tex_coord;

// Outgoing colour
layout (location = 0) out vec4 colour;

void main()
{
	// **************
	// Sample cubemap
	// **************
	colour = texture(cubemap, tex_coord);
}