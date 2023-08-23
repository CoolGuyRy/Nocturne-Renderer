#version 450

layout (location = 0) in vec2 fragUV;
layout (location = 1) in vec3 fragCol;

layout (set = 1, binding = 0) uniform sampler2D textureSampler;

layout (location = 0) out vec4 outColor;

void main(void) {
	if (fragUV.x == 0 && fragUV.y == 0) {
		outColor = vec4(fragCol.x, fragCol.y, fragCol.z, 1.0);
	} else {
		outColor = texture(textureSampler, fragUV);
	}
}