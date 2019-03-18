#version 410

// config
const int MAX_NUM_LIGHTS = 4;
const int MAX_NUM_PV_MATRICES = 6 * MAX_NUM_LIGHTS;

layout(triangles) in;
layout(triangle_strip, max_vertices=72) out; //18 * MAX_NUM_LIGHTS. Identifiers in layouts are unsupported

uniform mat4 matrixPV[MAX_NUM_PV_MATRICES];
uniform int numLights = 0;

out vec4 worldspacePosition;
flat out int lightIndex;

void main() {

    for (int layer = 0; layer < numLights; ++layer) {

        for (int face = 0; face < 6; ++face) {

            gl_Layer = layer * 6 + face;
            lightIndex = layer;

            for (int i = 0; i < 3; ++i) {

                worldspacePosition = gl_in[i].gl_Position;
                gl_Position = matrixPV[gl_Layer] * worldspacePosition;
                EmitVertex();
            }

            EndPrimitive();
        }
    }
}
