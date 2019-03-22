#version 400

const int NUM_DIRECTIONAL_LIGHTS = 4;
const int NUM_POINT_LIGHTS = 10;
const int NUM_SPOT_LIGHTS = 4;

in vec3 worldPosition;
in vec3 worldNormal;
in vec3 worldTangent;
in vec3 worldBitangent;
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

// Builtin uniforms
uniform DirectionalLight directionalLights[NUM_DIRECTIONAL_LIGHTS];
uniform sampler2DArrayShadow directionalDepthMaps;
uniform int numDirectionalLights = 0;

uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform samplerCubeArrayShadow depthCubeMaps;
uniform int numPointLights = 0;

uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform int numSpotLights = 0;

uniform vec3 viewPosition;

uniform vec3 ambientColor;

// Custom uniforms
uniform sampler2D albedoMap;
uniform sampler2D metallicSmoothnessMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;
uniform vec4 albedoColor = vec4(1,1,1,1);
uniform float metallicMultiplier = 1;
uniform float smoothnessMultiplier = 1;
uniform float aoMultiplier = 1;

const float PI = 3.14159265359;

out vec4 fragmentColor;

vec3 CalculateDirectionalLightContribution(int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao);
vec3 CalculatePointLightContribution(int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao);
vec3 CalculateSpotLightContribution (int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao);

vec3 GetNormal();
vec3 CookTorranceBRDF(vec3 N, vec3 V, vec3 L, float NdotL, vec3 albedo, float metallic, float roughness);
float CalculateDirectionalShadowMultiplier(int i, float cosTheta);
float CalculatePointShadowMultiplier(int i, vec3 fromLight, float distance, float cosTheta);
vec3 CalculateAmbientLighting(vec3 N, vec3 V, float NdotV, vec3 albedo, float metallic, float roughness, float ao);

float Pow5(float t) {
    float t2 = t * t;
    return t2 * t2 * t;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * Pow5(1.0 - cosTheta);
}

void main() {

    vec4 albedo = albedoColor * texture(albedoMap, texCoords);
#ifdef RENDER_MODE_CUTOUT
    if (albedo.a < 0.5f)
        discard;
#endif

    vec4  msSample  = texture(metallicSmoothnessMap, texCoords);
    float metallic  = metallicMultiplier * msSample.r;
    float roughness = 1.f - smoothnessMultiplier * msSample.a;
    float ao        = aoMultiplier * texture(aoMap, texCoords).r;
    vec3 normal = GetNormal();
    vec3 viewDirection = normalize(viewPosition - worldPosition);

    float NdotV = max(dot(normal, viewDirection), 0.0);

    vec3 color = CalculateAmbientLighting(normal, viewDirection, NdotV, albedo.rgb, metallic, roughness, ao);
    
    for (int i = 0; i < numDirectionalLights; ++i) {
        color += CalculateDirectionalLightContribution(i, normal, viewDirection, albedo.rgb, metallic, roughness, ao);
    }

    for (int i = 0; i < numPointLights; ++i) {
        color += CalculatePointLightContribution(i, normal, viewDirection, albedo.rgb, metallic, roughness, ao);
    }

    for (int i = 0; i < numSpotLights; ++i) {
        color += CalculateSpotLightContribution(i, normal, viewDirection, albedo.rgb, metallic, roughness, ao);
    }

    //float exposure = 2.2f;
    //color = vec3(1.0) - exp(-color * exposure);

#ifdef RENDER_MODE_FADE
    fragmentColor = vec4(color, albedo.a);
#else
    fragmentColor = vec4(color, 1);
#endif
}

vec3 GetNormal() {

    vec3 tangentspaceNormal = texture(normalMap, texCoords).xyz * 2.0 - 1.0;

    vec3 N = normalize(worldNormal);
    vec3 T = normalize(worldTangent);
    vec3 B = normalize(worldBitangent);
    return normalize(mat3(T,B,N) * tangentspaceNormal);
}

vec3 CalculateDirectionalLightContribution(int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao) {

    DirectionalLight light = directionalLights[i];

    vec3 L = -light.direction;
    float NdotL = max(dot(N, L), 0.0);
    vec3 brdf = CookTorranceBRDF(N, V, L, NdotL, albedo, metallic, roughness);

    float shadowMultiplier = CalculateDirectionalShadowMultiplier(i, NdotL);

    vec3 ambient = light.colorAmbient * albedo * ao;

    return ambient + brdf * light.color * NdotL * shadowMultiplier;
}

vec3 CalculatePointLightContribution(int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao) {

    PointLight light = pointLights[i];

    vec3  delta = light.position - worldPosition;
    float distance = length(delta);
    vec3  L = delta / distance;
    float attenuation = 1.f / (light.falloffConstant + light.falloffLinear * distance + light.falloffQuadratic * distance * distance);

    float NdotL = max(dot(N, L), 0.0);
    vec3 brdf = CookTorranceBRDF(N, V, L, NdotL, albedo, metallic, roughness);

    float shadowMultiplier = CalculatePointShadowMultiplier(i, -delta, distance, NdotL);
    vec3 ambient = light.colorAmbient * albedo * ao;
    return ambient + brdf * light.color * NdotL * attenuation * shadowMultiplier;
}

vec3 CalculateSpotLightContribution(int i, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao) {

    SpotLight light = spotLights[i];

    vec3  delta = light.position - worldPosition;
    float distance = length(delta);
    vec3  L = delta / distance;
    float attenuation = 1.f / (light.falloffConstant + light.falloffLinear * distance + light.falloffQuadratic * distance * distance);
    attenuation *= smoothstep(light.outerCutoff, light.innerCutoff, dot(-L, light.direction));

    float NdotL = max(dot(N, L), 0.0);
    vec3 brdf = CookTorranceBRDF(N, V, L, NdotL, albedo, metallic, roughness);

    vec3 ambient = light.colorAmbient * albedo * ao;
    return ambient + brdf * light.color * NdotL * attenuation;
}

float DistributionGGX(float NdotH, float roughness) {

    float a = roughness * roughness;
    float a2 = a * a;

    float nom   = a2;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return nom / denom;
}

// Incorporates the division by 4 * NdotL * NdotV
float GeometrySmith(float NdotL, float NdotV, float roughness) {

    float r = roughness + 1.0;
    float k = r * r * 0.125f; // r * r / 8.0
    float oneMinusK = 1.0 - k;
    return 0.25f / ((NdotL * oneMinusK + k) * (NdotV * oneMinusK + k) + 0.0001f);
}

float GeometrySmithIBL(float NdotL, float NdotV, float roughness) {

    float r = roughness;
    float k = (r * r) * 0.5f;
    float oneMinusK = 1.0 - k;
    return 0.25f / ((NdotL * oneMinusK + k) * (NdotV * oneMinusK + k) + 0.0001f);
}

vec3 CookTorranceBRDF(vec3 N, vec3 V, vec3 L, float NdotL, vec3 albedo, float metallic, float roughness) {

    vec3 H = normalize(V + L);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    float NDF = DistributionGGX(NdotH, roughness);
    float G   = GeometrySmith(NdotL, NdotV, roughness);
    vec3  F   = FresnelSchlick(HdotV, mix(vec3(0.04), albedo, metallic));

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    return kD * (albedo / PI) + kS * (NDF * G);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {

    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * Pow5(1.0 - cosTheta);
}

vec3 CalculateAmbientLighting(vec3 N, vec3 V, float NdotV, vec3 albedo, float metallic, float roughness, float ao) {

    vec3 H = normalize(V + N);
    float NdotH = max(dot(N, H), 0);
    float HdotV = max(dot(H, V), 0);

    vec3 kS = FresnelSchlickRoughness(HdotV, mix(vec3(0.04f), albedo, metallic), roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = ambientColor;
    vec3 diffuse = irradiance * albedo.xyz / PI;

    float NDF = DistributionGGX(NdotH, roughness);
    float G   = GeometrySmithIBL(1, NdotV, roughness);
    vec3 specular = ambientColor * NDF * G;

    return (kD * diffuse + kS * specular) * ao;
}

float CalculateDirectionalShadowMultiplier(int i, float cosTheta) {

    vec4 lightspacePosition = directionalLightspacePosition[i];
    vec3 projected = (lightspacePosition.xyz / lightspacePosition.w) * 0.5 + 0.5;
    float currentDepth = projected.z;
    if (currentDepth > 1.f)
        return 1;

    float bias = clamp(0.005 * tan(acos(cosTheta)), 0, 0.01);

    float shadow = 0.0;
    vec3 texelSize = 1.0 / textureSize(directionalDepthMaps, 0);
    vec2 scale = texelSize.xy * (1 + currentDepth * 0.1);
    for (float x = -1; x <= 1; x += 1)
        for (float y = -1; y <= 1; y += 1)
            shadow += texture(directionalDepthMaps, vec4(projected.xy + vec2(x, y) * scale, i, currentDepth - bias));
    shadow /= 9.0;

    return shadow;
}

const vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float CalculatePointShadowMultiplier(int i, vec3 fromLight, float distance, float cosTheta) {

    float shadow = 0.0;
    float bias = max(0.004 * (1 - cosTheta), 0.002);
    int numSamples = 1;
    float depth = distance / pointLights[i].farPlaneDistance - bias;
    float viewDistance = length(viewPosition - worldPosition);
    float diskRadius = (1.0 + (viewDistance / pointLights[i].farPlaneDistance)) / 25.0;

    //for (int j = 0; j < numSamples; ++j)
    //    shadow += texture(depthCubeMaps, vec4(fromLight + sampleOffsetDirections[j] * diskRadius, i), depth);
    //shadow /= float(numSamples);
    //return shadow;

    return texture(depthCubeMaps, vec4(fromLight, i), depth);
}