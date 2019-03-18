#version 330

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 matrixPVM;
uniform float time;

uniform float timeScale;
uniform float phaseOffsetPerUnitDistance;
uniform float wobbleMultiplierMin;
uniform float wobbleMultiplierMax;

out vec2 fUV;
out float wobbleFactor; // from -1 to 1. 0 is no wobble. 1 is max wobble outward, -1 is max wobble inward

void main() {

    float distance = vertex.y;
    wobbleFactor = sin(time * timeScale + distance * phaseOffsetPerUnitDistance);

    vec3 position = vertex * mix(wobbleMultiplierMin, wobbleMultiplierMax, smoothstep(-1, 1, wobbleFactor));
    gl_Position = matrixPVM * vec4(position, 1);
    fUV = uv;
}
