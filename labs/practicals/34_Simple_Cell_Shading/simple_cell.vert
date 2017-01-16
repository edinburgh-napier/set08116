#version 440

uniform mat4 MVP;

// Incoming vertex data
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

// Outgoing vertex data
layout (location = 0) out float tex_coord;

// Light direction
const vec3 light_dir = vec3(0, 0, 1.0);

void main()
{
	// Transform position
	gl_Position = MVP * vec4(position, 1.0);
	// Texture coordinate is a scalar based on angle between surface and light
	tex_coord = clamp(max(0.0, dot(normalize(normal), light_dir)), 0.0, 1.0);
}