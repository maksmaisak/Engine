#version 330

in vec2 fUV;
in float wobbleFactor; // See vertex shader for explanation

uniform sampler2D diffuseTexture;
uniform vec4 transitionColor;
uniform float transitionWobbleFactorMin;
uniform float transitionWobbleFactorMax;

out vec4 fragment_color;

void main() {

    float t = smoothstep(transitionWobbleFactorMin, transitionWobbleFactorMax, wobbleFactor);
    fragment_color = mix(texture(diffuseTexture, fUV), transitionColor, t);
}
