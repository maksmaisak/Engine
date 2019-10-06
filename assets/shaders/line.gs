#version 330

layout(lines) in;
layout(triangle_strip, max_vertices = 6) out;

in vec4 vertexColor[];
in float vertexLineHalfWidth[];
out vec4 interpolatedColor;

uniform vec2 viewportSize;
uniform vec2 inverseViewportSize;

void main() {

    vec4 start = gl_in[0].gl_Position;
    vec4 end   = gl_in[1].gl_Position;

    vec2 lineDirection = normalize((gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy) * viewportSize);
    vec2 baseHalfWidthDisplacement = vec2(-lineDirection.y, lineDirection.x) * inverseViewportSize;
    vec4 halfWidthDisplacementStart = vec4(baseHalfWidthDisplacement * vertexLineHalfWidth[0], 0.f, 0.f);
    vec4 halfWidthDisplacementEnd   = vec4(baseHalfWidthDisplacement * vertexLineHalfWidth[1], 0.f, 0.f);

    vec4 corner00 = start - halfWidthDisplacementStart;
    vec4 corner10 = start + halfWidthDisplacementStart;
    vec4 corner01 = end - halfWidthDisplacementEnd;
    vec4 corner11 = end + halfWidthDisplacementEnd;

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
