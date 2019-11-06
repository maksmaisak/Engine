#version 330

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;
uniform float threshold;

void main() {

	vec3 color = texture(sourceTexture, texCoords).rgb;
	float brightness = dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
	fragmentColor = vec4(step(threshold, brightness) * color, 1.f);
}
