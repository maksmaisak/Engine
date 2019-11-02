#version 330

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;

void main() {
	fragmentColor = texture(sourceTexture, texCoords);
}
