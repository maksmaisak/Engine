#version 330

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texCoords;

struct DirectionalLight {

    vec3 color;
    vec3 colorAmbient;

    vec3 direction;

    float falloffConstant;
    float falloffLinear;
    float falloffQuadratic;
};

struct PointLight {

    vec3 color;
    vec3 colorAmbient;

    vec3 position;

    float falloffConstant;
    float falloffLinear;
    float falloffQuadratic;
};

struct SpotLight {

    vec3 color;
    vec3 colorAmbient;

    vec3 position;
    vec3 direction;

    float falloffConstant;
    float falloffLinear;
    float falloffQuadratic;
    float innerCutoff; // cos of inner angle
    float outerCutoff; // cos of outer angle
};

uniform DirectionalLight directionalLights[4];
uniform int numDirectionalLights = 0;

uniform PointLight pointLights[10];
uniform int numPointLights = 0;

uniform SpotLight spotLights[4];
uniform int numSpotLights = 0;

uniform vec3 viewPosition;

uniform float time;

// Custom uniforms
// Diffuse
uniform vec3 diffuseColor  = vec3(1,1,1);
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;
uniform sampler2D splatmap;
// Specular
uniform vec3 specularColor = vec3(1,1,1);
uniform sampler2D specularMap;
uniform float shininess = 1;
// Animation
uniform sampler2D noise;

out vec4 fragmentColor;

vec3 calculateDirectionalLightContribution(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
vec3 calculatePointLightContribution(PointLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
vec3 calculateSpotLightContribution(SpotLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
vec3 getLightsContribution(vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);

vec4 animate(sampler2D sampler, vec2 uv) {

    float noiseValue = texture(noise, uv).r;
    float noisedTime = time + noiseValue;
    vec2 displacement = normalize(texCoords - vec2(0.5, 0.5)) * sin(noisedTime * 0.5) * 0.4;
    displacement.y = -displacement.y;

    float weightMultiplier = (1 + sin(time * 0.5) * 0.4);

    vec4 layer1 = texture(diffuse2, uv + displacement);
    vec4 layer2 = texture(diffuse2, uv - displacement);
    return mix(layer1, layer2, texture(noise, uv + displacement).r) * weightMultiplier;
}

vec3 sampleDiffuse(vec2 uv) {

    vec4 weights = texture(splatmap, texCoords);
    vec4 result =
        weights.r * texture(diffuse1, uv) +
        weights.g * animate(diffuse2, uv) +
        weights.b * texture(diffuse3, uv) +
        weights.a * texture(diffuse4, uv);

    return vec3(result);
}

vec3 getDiffuse(vec3 normal) {

    vec2 texCoordsX = worldPosition.zy;
    vec2 texCoordsY = worldPosition.xz;
    vec2 texCoordsZ = worldPosition.xy;

    vec3 sampleX = sampleDiffuse(texCoordsX);
    vec3 sampleY = sampleDiffuse(texCoordsY);
    vec3 sampleZ = sampleDiffuse(texCoordsZ);

    vec3 weights = abs(normal);
    weights /= (weights.x + weights.y + weights.z);
    return
        weights.x * sampleX +
        weights.y * sampleY +
        weights.z * sampleZ;
}

void main() {

    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 materialDiffuse  = diffuseColor  * getDiffuse(normal);
    vec3 materialSpecular = specularColor * vec3(texture(specularMap, texCoords));

    vec3 color = getLightsContribution(normal, viewDirection, materialDiffuse, materialSpecular);
	fragmentColor = vec4(color, 1);

	// uncomment one of these for debug view
	//fragmentColor = vec4((normal + vec3(1)) * 0.5, 1); // normals
	//fragmentColor = texture(splatmap, texCoords); // splatma,
	//fragmentColor = vec4(vec3(texture(splatmap, texCoords).b), 1); // splatmap channel
	//fragmentColor = vec4(vec3(length(texCoords - vec2(0.5, 0.5))), 1); // texcoord from center
}

vec3 getLightsContribution(vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    vec3 color = vec3(0,0,0);

    for (int i = 0; i < numDirectionalLights; ++i) {
        color += calculateDirectionalLightContribution(directionalLights[i], normal, viewDirection, materialDiffuse, materialSpecular);
    }

    for (int i = 0; i < numPointLights; ++i) {
        color += calculatePointLightContribution(pointLights[i], normal, viewDirection, materialDiffuse, materialSpecular);
    }

    for (int i = 0; i < numSpotLights; ++i) {
        color += calculateSpotLightContribution(spotLights[i], normal, viewDirection, materialDiffuse, materialSpecular);
    }

	return color;
}

vec3 calculateDirectionalLightContribution(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    vec3 ambient = light.colorAmbient * materialDiffuse;

    vec3 diffuse = max(0, dot(normal, light.direction)) * light.color * materialDiffuse;

    vec3 reflectedDirection = reflect(-light.direction, normal);
    vec3 specular = pow(max(0, dot(reflectedDirection, viewDirection)), shininess) * light.color * materialSpecular;

    return ambient + diffuse + specular;
}

vec3 calculatePointLightContribution(PointLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    vec3 ambient = light.colorAmbient * materialDiffuse;

    vec3 delta = light.position - worldPosition;
    float distance = length(delta);
    vec3 lightDirection = delta / distance;

    vec3 diffuse = max(0, dot(normal, lightDirection)) * light.color * materialDiffuse;

    vec3 reflectedDirection = reflect(-lightDirection, normal);
    vec3 specular = pow(max(0, dot(reflectedDirection, viewDirection)), shininess) * light.color * materialSpecular;

    float attenuation = 1.f / (light.falloffConstant + light.falloffLinear * distance + light.falloffQuadratic * distance * distance);

    return ambient + attenuation * (diffuse + specular);
}

vec3 calculateSpotLightContribution(SpotLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    vec3 ambient = light.colorAmbient * materialDiffuse;

    vec3 delta = light.position - worldPosition;
    float distance = length(delta);
    vec3 lightDirection = delta / distance;

    vec3 diffuse = max(0, dot(normal, lightDirection)) * light.color * materialDiffuse;

    vec3 reflectedDirection = reflect(-lightDirection, normal);
    vec3 specular = pow(max(0, dot(reflectedDirection, viewDirection)), shininess) * light.color * materialSpecular;

    float attenuation = 1.f / (light.falloffConstant + light.falloffLinear * distance + light.falloffQuadratic * distance * distance);
    attenuation *= smoothstep(light.outerCutoff, light.innerCutoff, dot(-lightDirection, light.direction));

    return ambient + attenuation * (diffuse + specular);
}