#version 330

in vec2 texCoords;
out vec4 fragmentColor;

uniform sampler2D sourceTexture;

void main()
{
	float texelSizeY = 1.f / textureSize(sourceTexture, 0).y;

	const float weight[5] = float[] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

	vec3 result = texture(sourceTexture, texCoords).rgb * weight[0];
	for (int i = 1; i < 5; ++i)
	{
		result += texture(sourceTexture, texCoords + vec2(0.f, texelSizeY * i)).rgb * weight[i];
		result += texture(sourceTexture, texCoords - vec2(0.f, texelSizeY * i)).rgb * weight[i];
	}

	fragmentColor = vec4(result, 1.f);
}
