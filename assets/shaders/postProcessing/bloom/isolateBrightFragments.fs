#version 330

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;

void main() {

	const float threshold = 0.5f;

	vec3 color = texture(sourceTexture, texCoords).rgb;
	float brightness = dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
	//fragmentColor = vec4(step(threshold, brightness) * color, 1.f);
	fragmentColor = vec4(brightness > threshold ? color : vec3(0.f, 0.f, 0.f), 1.f);
}
