#version 450

layout (location = 0) in vec3 fragColor;	// Interpolated Color from Vertex (Location must match)

layout (location = 0) out vec4 outColor;	// Final output color (must also have location)

void main(void) {
	outColor = vec4(fragColor, 1.0);
}
