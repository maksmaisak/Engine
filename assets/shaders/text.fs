#version 330

in vec2 texCoords;

uniform sampler2D fontAtlas;
uniform vec4 textColor;

out vec4 color;

void main() {

    color = textColor * texture(fontAtlas, texCoords);
}