#version 330

layout(lines) in;
layout(triangle_strip, max_vertices = 6) out;

in vec4 vertexColor[];
out vec4 interpolatedColor;

uniform vec2 halfWidthInClipspace;

vec4 getHalfWidthDisplacement(vec4 delta) {

    vec2 halfWidthDisplacement = normalize(vec2(-delta.y, delta.x)) * halfWidthInClipspace;
    return vec4(halfWidthDisplacement.x, halfWidthDisplacement.y, delta.z, delta.w);
}

void main() {

    vec4 start = gl_in[0].gl_Position;
    vec4 end = gl_in[1].gl_Position;
    vec4 delta = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 halfWidthDisplacement = getHalfWidthDisplacement(delta);

    vec4 corner00 = start - halfWidthDisplacement;
    vec4 corner10 = start + halfWidthDisplacement;
    vec4 corner01 = end - halfWidthDisplacement;
    vec4 corner11 = end + halfWidthDisplacement;

    interpolatedColor = vertexColor[0];
    gl_Position = corner00; EmitVertex();
    gl_Position = corner10; EmitVertex();
    interpolatedColor = vertexColor[1];
    gl_Position = corner11; EmitVertex();

    interpolatedColor = vertexColor[0];
    gl_Position = corner00; EmitVertex();
    interpolatedColor = vertexColor[1];
    gl_Position = corner11; EmitVertex();
    gl_Position = corner01; EmitVertex();

    EndPrimitive();
}
