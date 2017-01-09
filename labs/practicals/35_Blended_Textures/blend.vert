#version 440

uniform mat4 MVP;

// *********************************
// Declare incoming values
// 0 - position

// 10 - tex_coord_in

// *********************************

// Outgoing value
layout (location = 0) out vec2 tex_coord_out;

void main()
{
	// Transform the position onto screen
	gl_Position = MVP * vec4(position, 1.0);
	// Output texture coordinates
	tex_coord_out = tex_coord_in;
} 