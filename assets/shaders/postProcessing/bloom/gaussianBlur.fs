#version 330
// One part (either horizontal or vertical) of the two-pass gaussian blur.

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;
uniform int kernelSize;
uniform float kernel[64];
uniform bool isHorizontal;

void main()
{
	vec2 texelSize = 1.f / textureSize(sourceTexture, 0).xy;
	vec2 oneTexelDelta = isHorizontal ? vec2(texelSize.x, 0.f) : vec2(0.f, texelSize.y);

	vec3 result = texture(sourceTexture, texCoords).rgb * kernel[0];
	for (int i = 1; i < kernelSize; ++i)
	{
		vec2 delta = oneTexelDelta * i;
		result += texture(sourceTexture, texCoords + delta).rgb * kernel[i];
		result += texture(sourceTexture, texCoords - delta).rgb * kernel[i];
	}

	fragmentColor = vec4(result, 1.f);
}
