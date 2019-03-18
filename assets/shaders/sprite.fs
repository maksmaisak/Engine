#version 330

in vec2 texCoords;

uniform sampler2D spriteTexture;
uniform vec4 spriteColor;

out vec4 color;

void main() {

    color = texture(spriteTexture, texCoords) * spriteColor;
}  