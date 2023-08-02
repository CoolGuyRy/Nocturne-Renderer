#version 450

layout (location = 0) in vec3 fragCol;
layout (location = 1) in vec2 fragTex;

layout (set = 1, binding = 0) uniform sampler2D textureSampler;

layout (location = 0) out vec4 outColor;	// Final output color (must also have location)

void main(void) {
	//outColor = vec4(fragCol, 1.0);
	//outColor = texture(textureSampler, fragTex);
	outColor = texture(textureSampler, fragTex) * vec4(fragCol * 0.75, 1.0);
}