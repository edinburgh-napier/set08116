#version 440

vec3 calc_normal(in vec3 normal, in vec3 tangent, in vec3 binormal, in sampler2D normal_map, in vec2 tex_coord)
{
	// *********************************
	// Ensure normal, tangent and binormal are unit length (normalized)



	// Sample normal from normal map

	// *********************************
	// Transform components to range [0, 1]
	sampled_normal = 2.0 * sampled_normal - vec3(1.0, 1.0, 1.0);
	// Generate TBN matrix
	mat3 TBN = mat3(tangent, binormal, normal);
	// Return sampled normal transformed by TBN
	return normalize(TBN * sampled_normal);
}