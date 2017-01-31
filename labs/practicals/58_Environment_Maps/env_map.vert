#version 440

// MVP transformation matrix
uniform mat4 MVP;
// M transformation matrix
uniform mat4 M;
// N transformation matrix
uniform mat3 N;
// Eye position
uniform vec3 eye_pos;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;

// Outgoing 3D texture coordinate
layout (location = 0) out vec3 tex_coord;

void main()
{
	// Calculate screen position
	gl_Position = MVP * vec4(position, 1.0);
	// ************************
	// Calculate world position
	// ************************
	vec3 transformed_position = (M * vec4(position, 1.0)).xyz;
	// ********************
	// Transform the normal
	// ********************
	vec3 transformed_normal = N * normal;
	// ****************************************************************
	// Calculate tex_coord using world position and transformed normal
	// ****************************************************************
	tex_coord = normalize(reflect(transformed_position - eye_pos, transformed_normal));
}