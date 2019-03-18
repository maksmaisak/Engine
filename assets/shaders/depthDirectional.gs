#version 330

#define MAX_NUM_LIGHTS 10

layout(triangles) in;
layout(triangle_strip, max_vertices = 30) out; // 3 * MAX_NUM_LIGHTS. identifiers in layout are unsupported

uniform mat4 matrixPV[MAX_NUM_LIGHTS];
uniform int numLights = 0;

void main() {

    for (int lightIndex = 0; lightIndex < numLights; ++lightIndex) {

        gl_Layer = lightIndex;
        for (int i = 0; i < 3; ++i) {
            gl_Position = matrixPV[gl_Layer] * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}
