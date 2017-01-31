#version 410

// Incoming position
layout (location = 0) in vec3 position_in;
// Incoming velocity
layout (location = 1) in vec3 velocity_in;

// Outgoing position
layout (location = 0) out vec3 position_out;
// Outgoing velocity
layout (location = 1) out vec3 velocity_out;

void main()
{
    // Pass through the values
    position_out = position_in;
    velocity_out = velocity_in;
}