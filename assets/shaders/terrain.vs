#version 330

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 matrixProjection;
uniform	mat4 matrixView;
uniform	mat4 matrixModel;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texCoords;

uniform sampler2D heightmap;
uniform float maxHeight = 1;

float sampleHeight(vec2 heightmapUv) {
    return texture(heightmap, heightmapUv).r * maxHeight;
}

vec3 sampleDisplacement(vec2 heightmapUv) {
    return vec3(0, sampleHeight(heightmapUv), 0);
}

vec3 samplePositionUVSpace(vec2 heightmapUv) {
    return vec3(heightmapUv.x, sampleHeight(heightmapUv), heightmapUv.y);
}

vec3 sampleModelSpace(vec2 heightmapUv) {

    vec3 uvspacePosition = samplePositionUVSpace(heightmapUv);
    return vec3(
        uvspacePosition.x * 2.f - 1.f,
        uvspacePosition.y,
        uvspacePosition.z * -2.f - 1.f
    );
}

void main(void) {

    worldPosition = vec3(matrixModel * vec4(vertex + sampleDisplacement(uv), 1));
    gl_Position = matrixProjection * matrixView * vec4(worldPosition, 1);

    vec3 modelspacePosition = sampleModelSpace(uv);
    vec3 tangentX = sampleModelSpace(uv + vec2(0.01, 0)) - modelspacePosition;
    vec3 tangentZ = sampleModelSpace(uv + vec2(0, 0.01)) - modelspacePosition;
    vec3 computedNormal = normalize(cross(normalize(tangentZ), normalize(tangentX)));

    //worldNormal = computedNormal;
    worldNormal = mat3(transpose(inverse(matrixModel))) * computedNormal;

    texCoords = uv;
}
