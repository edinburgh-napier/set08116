#version 440

uniform mat4 MVP;
uniform vec2 UV_SCROLL;

layout (location = 0) in vec3 position;
layout (location = 10) in vec2 tex_coord_in;

layout (location = 0) out vec2 tex_coord_out;

void main()
{
	// Transform position
	gl_Position = MVP * vec4(position, 1.0);
	// Pass through texture coordinate
	tex_coord_out = tex_coord_in + UV_SCROLL;
}