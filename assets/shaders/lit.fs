#version 400

const int NUM_DIRECTIONAL_LIGHTS = 4;
const int NUM_POINT_LIGHTS = 10;
const int NUM_SPOT_LIGHTS = 4;

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texCoords;
in vec4 directionalLightspacePosition[NUM_DIRECTIONAL_LIGHTS];

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

    float farPlaneDistance;
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

uniform DirectionalLight directionalLights[NUM_DIRECTIONAL_LIGHTS];
uniform sampler2DArrayShadow directionalDepthMaps;
uniform int numDirectionalLights = 0;

uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform samplerCubeArrayShadow depthCubeMaps;
uniform int numPointLights = 0;

uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform int numSpotLights = 0;

uniform vec3 viewPosition;

// Custom uniforms
uniform vec3 diffuseColor  = vec3(1,1,1);
uniform sampler2D diffuseMap;
uniform vec3 specularColor = vec3(1,1,1);
uniform sampler2D specularMap;
uniform float shininess = 1;

out vec4 fragmentColor;

vec3 CalculateDirectionalLightContribution(int i, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
vec3 CalculatePointLightContribution(int i, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
vec3 CalculateSpotLightContribution(SpotLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular);
float CalculateDirectionalShadowMultiplier(int i, float biasMultiplier);
float CalculatePointShadowMultiplier(int i, vec3 fromLight, float distance, float biasMultiplier);

void main() {

    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 materialDiffuse  = diffuseColor  * vec3(texture(diffuseMap , texCoords));
    vec3 materialSpecular = specularColor * vec3(texture(specularMap, texCoords));

    vec3 color = vec3(0,0,0);

    for (int i = 0; i < numDirectionalLights; ++i) {
        color += CalculateDirectionalLightContribution(i, normal, viewDirection, materialDiffuse, materialSpecular);
    }

    for (int i = 0; i < numPointLights; ++i) {
        color += CalculatePointLightContribution(i, normal, viewDirection, materialDiffuse, materialSpecular);
    }

    for (int i = 0; i < numSpotLights; ++i) {
        color += CalculateSpotLightContribution(spotLights[i], normal, viewDirection, materialDiffuse, materialSpecular);
    }

	fragmentColor = vec4(color, 1);
}

vec3 CalculateDirectionalLightContribution(int i, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    DirectionalLight light = directionalLights[i];

    vec3 ambient = light.colorAmbient * materialDiffuse;

    float normalDotDirection = dot(normal, light.direction);
    vec3 diffuse = max(0, normalDotDirection) * materialDiffuse;

    vec3 reflectedDirection = reflect(-light.direction, normal);
    vec3 specular = pow(max(0, dot(reflectedDirection, viewDirection)), shininess) * materialSpecular;

    float shadowMultiplier = CalculateDirectionalShadowMultiplier(i, 1 - normalDotDirection);
    return ambient + shadowMultiplier * (diffuse + specular) * light.color;
}

vec3 CalculatePointLightContribution(int i, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

    PointLight light = pointLights[i];

    vec3 ambient = light.colorAmbient * materialDiffuse;

    vec3 delta = light.position - worldPosition;
    float distance = length(delta);
    vec3 lightDirection = delta / distance;

    float normalDotDirection = dot(normal, lightDirection);
    vec3 diffuse = max(0, normalDotDirection) * materialDiffuse;

    vec3 reflectedDirection = reflect(-lightDirection, normal);
    vec3 specular = pow(max(0, dot(reflectedDirection, viewDirection)), shininess) * materialSpecular;

    float attenuation = 1.f / (light.falloffConstant + light.falloffLinear * distance + light.falloffQuadratic * distance * distance);
    float shadowMultiplier = CalculatePointShadowMultiplier(i, -delta, distance, 1 - normalDotDirection);
    //return texture(depthMap, -delta).r * vec3(1);
    //return shadowMultiplier * vec3(1) / numPointLights;
    return ambient + shadowMultiplier * attenuation * (diffuse + specular) * light.color;
}

vec3 CalculateSpotLightContribution(SpotLight light, vec3 normal, vec3 viewDirection, vec3 materialDiffuse, vec3 materialSpecular) {

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

float CalculateDirectionalShadowMultiplier(int i, float biasMultiplier) {

    vec4 lightspacePosition = directionalLightspacePosition[i];
    vec3 projected = (lightspacePosition.xyz / lightspacePosition.w) * 0.5 + 0.5;
    float currentDepth = projected.z;
    if (currentDepth > 1.f)
        return 1;

    float bias = max(0.002 * biasMultiplier, 0.001);
    return texture(directionalDepthMaps, vec4(projected.xy, i, currentDepth - bias));
}

float CalculatePointShadowMultiplier(int i, vec3 fromLight, float distance, float biasMultiplier) {

    float bias = max(0.1 * biasMultiplier, 0.05);
    return texture(depthCubeMaps, vec4(fromLight, i), (distance - bias) / pointLights[i].farPlaneDistance);
}