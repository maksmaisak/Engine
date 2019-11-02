#version 330

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;
uniform sampler2D blurredTexture;
uniform float intensity;

void main() {

	vec3 sourceColor = texture(sourceTexture, texCoords).rgb;
	vec3 blurredColor = intensity * texture(blurredTexture, texCoords).rgb;
	//fragmentColor = texture(sourceTexture, texCoords);
	//fragmentColor = vec4(blurredColor, 1.f);
	fragmentColor = vec4(sourceColor + blurredColor, 1.f);
}
