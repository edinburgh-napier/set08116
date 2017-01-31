#version 440

// The transformation matrix
uniform mat4 MVP;
// The model transformation
uniform mat4 M;
// The normal matrix
uniform mat3 N;
// The light transformation matrix
uniform mat4 lightMVP;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinate
layout (location = 10) in vec2 tex_coord_in;

// Outgoing vertex position
layout (location = 0) out vec3 vertex_position;
// Outgoing normal
layout (location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;
// Outgoing position in light space
layout (location = 3) out vec4 vertex_light;

void main()
{
    // Transform position into screen space
    gl_Position = MVP * vec4(position, 1.0);
    // Transform position into world space
    vertex_position = (M * vec4(position, 1.0)).xyz;
    // Transform normal
    transformed_normal = N * normal;
    // Pass through texture coordinate
    tex_coord_out = tex_coord_in;
    // *********************************
    // Transform position into light space

    // *********************************
}