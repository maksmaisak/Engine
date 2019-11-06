#version 330
// Remaps colors from the sourceTexture into the [0..1] range. Everything is done in linear color space.

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;
uniform float exposure;

void main() {

	vec3 hdrColor = texture(sourceTexture, texCoords).rgb;
	vec3 mapped = vec3(1.f) - exp(-hdrColor * exposure);

	fragmentColor = vec4(mapped, 1.f);
}
