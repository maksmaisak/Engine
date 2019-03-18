#version 410

const int NUM_DIRECTIONAL_LIGHTS = 4;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 matrixProjection;
uniform	mat4 matrixView;
uniform	mat4 matrixModel;
uniform mat4 directionalLightspaceMatrix[NUM_DIRECTIONAL_LIGHTS];
uniform int numDirectionalLights = 0;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoords;

out vec4 directionalLightspacePosition[NUM_DIRECTIONAL_LIGHTS];

void main(void) {

    worldPosition = vec3(matrixModel * vec4(vertex, 1));
    gl_Position = matrixProjection * matrixView * vec4(worldPosition, 1);

    worldNormal = mat3(transpose(inverse(matrixModel))) * normal;
    texCoords = uv;

    for (int i = 0; i < numDirectionalLights; ++i) {
        directionalLightspacePosition[i] = directionalLightspaceMatrix[i] * vec4(worldPosition, 1);
    }
}
