//
// Created by Maksym Maisak on 2019-01-09.
//

#include <cassert>
#include <iostream>
#include <utility>
#include <map>
#include "Transform.h"
#include "Light.h"
#include "Material.h"
#include "Resources.h"
#include "Mesh.h"
#include "DepthMaps.h"
#include "GLHelpers.h"
#include "GLSetUniform.h"
#include "TupleUtils.h"
#include "GameTime.h"
#include "LuaState.h"
#include "Font.h"

using namespace en;
using namespace std::string_literals;

Material::Material(const std::string& shaderFilename) :
    Material(Resources<ShaderProgram>::get(shaderFilename))
{}

Material::Material(std::shared_ptr<ShaderProgram> shader) : m_shader(std::move(shader)) {

    assert(m_shader);

    m_builtinUniformLocations = cacheBuiltinUniformLocations();
    m_attributeLocations = cacheAttributeLocations();
    detectAllUniforms();
}

namespace {

    std::string getShaderNameFromLua(LuaState& lua) {

        if (!lua_istable(lua, -1) && !lua_isuserdata(lua, -1)) {
            luaL_error(lua, "Can't make a material out of given %s", luaL_typename(lua, -1));
        }

        return lua.tryGetField<std::string>("shader").value_or("lit");
    }

    std::shared_ptr<ShaderProgram> getShader(LuaState& lua) {

        const std::string shaderName = getShaderNameFromLua(lua);
        if (shaderName != "pbr") {
            return Resources<ShaderProgram>::get(shaderName);
        }

        // Special handling for "pbr" to handle conditionally-compiled shader variants

        const std::string renderMode = lua.tryGetField<std::string>("renderMode").value_or("opaque");

        using Defs = PreprocessorDefinitions;
        static const std::map<std::string, std::function<std::shared_ptr<ShaderProgram>()>> renderModeGetters = {
            {"opaque", [](){return Resources<ShaderProgram>::get("pbr");}},
            {"cutout", [](){return Resources<ShaderProgram>::get("pbr-cutout", "pbr", Defs{{"RENDER_MODE_CUTOUT"}});}},
            {"fade"  , [](){return Resources<ShaderProgram>::get("pbr-fade"  , "pbr", Defs{{"RENDER_MODE_FADE"  }});}}
        };

        auto it = renderModeGetters.find(renderMode);
        if (it == renderModeGetters.end()) {
            throw utils::Exception("Unknown renderMode for a material: " + renderMode);
        }

        const auto& get = it->second;
        return get();
    }

    std::shared_ptr<Texture> getTextureFromLua(LuaState& lua, const std::string& fieldName, const std::shared_ptr<Texture>& defaultTexture = Textures::white(), GLenum textureInternalFormat = GL_SRGB8_ALPHA8) {

        std::optional<std::string> path = lua.tryGetField<std::string>(fieldName);
        if (path) {
            Texture::CreationSettings settings;
            settings.internalFormat = textureInternalFormat;
            return Textures::get(config::ASSETS_PATH + *path, settings);
        }

        return defaultTexture;
    }

    // Readers of properties for different shaders.
    static std::map<std::string, std::function<void(LuaState&, Material&)>> readers = {
        {
            "lit",
            [](LuaState& lua, Material& m) {

                m.setUniformValue("diffuseColor" , lua.tryGetField<glm::vec3>("diffuseColor").value_or(glm::vec3(1.f)));
                m.setUniformValue("specularColor", lua.tryGetField<glm::vec3>("specularColor").value_or(glm::vec3(1.f)));
                m.setUniformValue("shininess"    , lua.tryGetField<float>("shininess").value_or(10.f));

                m.setUniformValue("diffuseMap" , getTextureFromLua(lua, "diffuse"));
                m.setUniformValue("specularMap", getTextureFromLua(lua, "specular"));
            }
        },
        {
            "pbr",
            [](LuaState& lua, Material& m) {

                m.setUniformValue("albedoMap", getTextureFromLua(lua, "albedo", Textures::white()));

                const auto defaultMetallicSmoothnessMap = Textures::white();
                const auto metallicSmoothnessMap = getTextureFromLua(lua, "metallicSmoothness", defaultMetallicSmoothnessMap, GL_RGBA8);
                const bool isDefaultMSMap = metallicSmoothnessMap == defaultMetallicSmoothnessMap;
                m.setUniformValue("metallicSmoothnessMap", metallicSmoothnessMap);

                m.setUniformValue("aoMap"       , getTextureFromLua(lua, "ao"    , Textures::white(), GL_RGBA8));
                m.setUniformValue("normalMap"   , getTextureFromLua(lua, "normal", Textures::defaultNormalMap(), GL_RGBA8));

                m.setUniformValue("albedoColor", lua.tryGetField<glm::vec4>("albedoColor").value_or(glm::vec4(1)));
                m.setUniformValue("metallicMultiplier"  , lua.tryGetField<float>("metallicMultiplier").value_or(isDefaultMSMap ? 0 : 1));
                m.setUniformValue("smoothnessMultiplier", lua.tryGetField<float>("smoothnessMultiplier").value_or(isDefaultMSMap ? 0.5 : 1));
                m.setUniformValue("aoMultiplier"        , lua.tryGetField<float>("aoMultiplier").value_or(1));
            }
        },
        {
            "color",
            [](LuaState& lua, Material& m) {
                m.setUniformValue("diffuseColor", lua.tryGetField<glm::vec3>("color").value_or(glm::vec3(1.f)));
            }
        },
        {
            "sprite",
            [](LuaState& lua, Material& m) {
                m.setUniformValue("spriteTexture", getTextureFromLua(lua, "texture"));
                m.setUniformValue("spriteColor", lua.tryGetField<glm::vec4>("color").value_or(glm::vec4(1.f)));
            }
        }
    };
}

Material::Material(LuaState& lua) : Material(getShader(lua)) {

    const std::string shaderName = getShaderNameFromLua(lua);

    const auto it = readers.find(shaderName);
    if (it == readers.end()) {
        return;
    }

    const auto& readFunction = it->second;
    readFunction(lua, *this);
}

void Material::use(
    Engine* engine,
    DepthMaps* depthMaps,
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix
) {
    m_shader->use();
    m_numTexturesInUse = 0;
    setBuiltinUniforms(engine, depthMaps, modelMatrix, viewMatrix, projectionMatrix);
    setCustomUniforms();
}

void Material::render(
    const Mesh* mesh,
    Engine* engine,
    DepthMaps* depthMaps,
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix
) {
    use(engine, depthMaps, modelMatrix, viewMatrix, projectionMatrix);
    setAttributesAndDraw(mesh);
}

void Material::setAttributesAndDraw(const Mesh* mesh) {

    mesh->render(
        m_attributeLocations.vertex,
        m_attributeLocations.normal,
        m_attributeLocations.uv,
        m_attributeLocations.tangent,
        m_attributeLocations.bitangent
    );
}

namespace {

    inline bool valid(GLint location) { return location != -1; }

    glm::vec3 getAmbientColor(Engine& engine) {

        Scene* scene = engine.getSceneManager().getCurrentScene();
        if (!scene) {
            return glm::vec3(0.f);
        }

        return scene->getRenderSettings().ambientColor;
    }
}

void Material::setBuiltinUniforms(
    Engine* engine,
    DepthMaps* depthMaps,
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix
) {
    const auto& u = m_builtinUniformLocations;

    updateModelMatrix(modelMatrix);
    // glUniform functions do nothing if location is -1, so checks are only necessary for avoiding calculations.
    gl::setUniform(u.view      , viewMatrix      );
    gl::setUniform(u.projection, projectionMatrix);

    if (valid(u.pvm))
        gl::setUniform(u.pvm, projectionMatrix * viewMatrix * modelMatrix);

    if (valid(u.modelNormal))
        gl::setUniform(u.modelNormal, glm::mat3(glm::transpose(glm::inverse(modelMatrix))));

    if (valid(u.time))
        gl::setUniform(u.time, GameTime::asSeconds(GameTime::sinceAppStart()));

    if (valid(u.viewPosition))
        gl::setUniform(u.viewPosition, glm::vec3(glm::inverse(viewMatrix)[3]));

    if (valid(u.ambientColor))
        gl::setUniform(u.ambientColor, getAmbientColor(*engine));

    if (depthMaps && valid(u.directionalDepthMaps))
        setUniformTexture(u.directionalDepthMaps, depthMaps->getDirectionalMapsTexture(), GL_TEXTURE_2D_ARRAY);

    if (depthMaps && valid(u.depthCubemaps))
        setUniformTexture(u.depthCubemaps, depthMaps->getCubemapsTexture(), GL_TEXTURE_CUBE_MAP_ARRAY);

    auto& registry = engine->getRegistry();

    // Add lights
    int numPointLights = 0;
    int numDirectionalLights = 0;
    int numSpotLights = 0;
    for (Entity e : registry.with<Transform, Light>()) {

        // TODO sort lights by proximity, pick the closest ones.

        auto& light = registry.get<Light>(e);
        auto& tf = registry.get<Transform>(e);

        switch (light.kind) {
            case Light::Kind::POINT:
                if (numPointLights >= m_numSupportedPointLights)
                    break;
                setUniformsPointLight(u.pointLights[numPointLights], light, tf);
                numPointLights += 1;
                break;
            case Light::Kind::DIRECTIONAL:
                if (numDirectionalLights >= m_numSupportedDirectionalLights)
                    break;
                setUniformDirectionalLight(u.directionalLights[numDirectionalLights], light, tf);
                numDirectionalLights += 1;
                break;
            case Light::Kind::SPOT:
                if (numSpotLights >= m_numSupportedSpotLights)
                    break;
                setUniformSpotLight(u.spotLights[numSpotLights], light, tf);
                numSpotLights += 1;
                break;
            default:
                break;
        }
    }
    gl::setUniform(u.numPointLights, numPointLights);
    gl::setUniform(u.numDirectionalLights, numDirectionalLights);
    gl::setUniform(u.numSpotLights, numSpotLights);
}

void Material::updateModelMatrix(const glm::mat4& modelMatrix) {

    const auto& u = m_builtinUniformLocations;

    gl::setUniform(u.model, modelMatrix);

    if (valid(u.modelNormal)) {
        gl::setUniform(u.modelNormal, glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
    }
}

template<typename T>
void Material::setCustomUniformsOfType(const Material::LocationToUniformValue<T>& values) {

    for (auto& [location, value] : values) {
        gl::setUniform(location, value);
    }
}

template<>
void Material::setCustomUniformsOfType<std::shared_ptr<Texture>>(const Material::LocationToUniformValue<std::shared_ptr<Texture>>& values) {

    for (auto& [location, texture] : values) {
        if (!setUniformTexture(location, texture->getGLTextureObject())) {
            break;
        }
    }
}

template<>
void Material::setCustomUniformsOfType<Material::FontAtlas>(const Material::LocationToUniformValue<FontAtlas>& values) {

    for (auto& [location, setting] : values) {
        if (!setUniformTexture(location, setting.font->getTexture(setting.characterSize).getGLTextureObject())) {
            break;
        }
    }
}

void Material::setCustomUniforms() {

    std::apply([this](auto&&... args){(setCustomUniformsOfType(args), ...);}, m_uniformValues);
}

bool Material::setUniformTexture(GLint uniformLocation, GLuint textureId, GLenum textureTarget) {

    if (m_numTexturesInUse >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {

        // TODO have materials (and other objects) have a name to display in these error messages.
        std::cout << "Too many textures for this material: " << m_numTexturesInUse << "/" << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS << std::endl;
        return false;
    }

    glActiveTexture(GL_TEXTURE0 + m_numTexturesInUse);
    glBindTexture(textureTarget, textureId);
    glUniform1i(uniformLocation, m_numTexturesInUse);

    //std::cout << "Set texture to uniform. Texture unit: " << m_numTexturesInUse << ", textureId: " << textureId << ", textureTarget: " << textureTarget << ", uniform location: " << uniformLocation << std::endl;

    m_numTexturesInUse += 1;
    return true;
}

Material::BuiltinUniformLocations Material::cacheBuiltinUniformLocations() {

    BuiltinUniformLocations u;

    u.model       = m_shader->getUniformLocation("matrixModel");
    u.view        = m_shader->getUniformLocation("matrixView");
    u.projection  = m_shader->getUniformLocation("matrixProjection");
    u.pvm         = m_shader->getUniformLocation("matrixPVM");
    u.modelNormal = m_shader->getUniformLocation("matrixModelNormal");
    u.time        = m_shader->getUniformLocation("time");
    u.viewPosition = m_shader->getUniformLocation("viewPosition");
    u.directionalDepthMaps = m_shader->getUniformLocation("directionalDepthMaps");
    u.depthCubemaps = m_shader->getUniformLocation("depthCubeMaps");
    u.ambientColor  = m_shader->getUniformLocation("ambientColor");

    // Point lights
    u.numPointLights = m_shader->getUniformLocation("numPointLights");
    for (int i = 0; i < MAX_NUM_POINT_LIGHTS; ++i) {

        const std::string prefix = "pointLights[" + std::to_string(i) + "].";
        auto& locations = u.pointLights[i];

        locations.color = m_shader->getUniformLocation(prefix + "color");
        if (locations.color == -1)
            break;

        locations.colorAmbient = m_shader->getUniformLocation(prefix + "colorAmbient");
        locations.position     = m_shader->getUniformLocation(prefix + "position");
        locations.falloffConstant  = m_shader->getUniformLocation(prefix + "falloffConstant");
        locations.falloffLinear    = m_shader->getUniformLocation(prefix + "falloffLinear");
        locations.falloffQuadratic = m_shader->getUniformLocation(prefix + "falloffQuadratic");
        locations.farPlaneDistance = m_shader->getUniformLocation(prefix + "farPlaneDistance");

        m_numSupportedPointLights = i + 1;
    }

    // Directional lights
    u.numDirectionalLights = m_shader->getUniformLocation("numDirectionalLights");
    for (int i = 0; i < MAX_NUM_DIRECTIONAL_LIGHTS; ++i) {

        const std::string prefix = "directionalLights[" + std::to_string(i) + "].";
        auto& locations = u.directionalLights[i];

        locations.color = m_shader->getUniformLocation(prefix + "color");
        if (locations.color == -1)
            break;

        locations.colorAmbient = m_shader->getUniformLocation(prefix + "colorAmbient");
        locations.direction    = m_shader->getUniformLocation(prefix + "direction");
        locations.falloffConstant  = m_shader->getUniformLocation(prefix + "falloffConstant");
        locations.falloffLinear    = m_shader->getUniformLocation(prefix + "falloffLinear");
        locations.falloffQuadratic = m_shader->getUniformLocation(prefix + "falloffQuadratic");
        locations.lightspaceMatrix = m_shader->getUniformLocation("directionalLightspaceMatrix[" + std::to_string(i) + "]");

        m_numSupportedDirectionalLights = i + 1;
    }

    // Spot lights
    u.numSpotLights = m_shader->getUniformLocation("numSpotLights");
    for (int i = 0; i < MAX_NUM_SPOT_LIGHTS; ++i) {

        const std::string prefix = "spotLights[" + std::to_string(i) + "].";
        auto& locations = u.spotLights[i];

        locations.color = m_shader->getUniformLocation(prefix + "color");
        if (locations.color == -1)
            break;

        locations.colorAmbient = m_shader->getUniformLocation(prefix + "colorAmbient");
        locations.position     = m_shader->getUniformLocation(prefix + "position");
        locations.direction    = m_shader->getUniformLocation(prefix + "direction");
        locations.falloffConstant  = m_shader->getUniformLocation(prefix + "falloffConstant");
        locations.falloffLinear    = m_shader->getUniformLocation(prefix + "falloffLinear");
        locations.falloffQuadratic = m_shader->getUniformLocation(prefix + "falloffQuadratic");
        locations.innerCutoff      = m_shader->getUniformLocation(prefix + "innerCutoff");
        locations.outerCutoff      = m_shader->getUniformLocation(prefix + "outerCutoff");

        m_numSupportedSpotLights = i + 1;
    }

    return u;
}

Material::AttributeLocations Material::cacheAttributeLocations() {

    AttributeLocations a;

    a.vertex    = m_shader->getAttributeLocation("vertex");
    a.normal    = m_shader->getAttributeLocation("normal");
    a.uv        = m_shader->getAttributeLocation("uv");
    a.tangent   = m_shader->getAttributeLocation("tangent");
    a.bitangent = m_shader->getAttributeLocation("bitangent");

    return a;
}

void Material::detectAllUniforms() {

    std::vector<UniformInfo> uniforms = m_shader->getAllUniforms();

    //std::cout << "Uniforms: " << std::endl;
    for (const UniformInfo& info : uniforms) {
        //std::cout << info.location << " : " << info.name << std::endl;
        m_uniforms[info.name] = info;
    }
    //std::cout << std::endl;
}

void Material::setUniformsPointLight(
    const BuiltinUniformLocations::PointLightLocations& locations,
    const Light& light,
    const Transform& tf
) {
    if (valid(locations.position)) {
        gl::setUniform(locations.position, tf.getWorldPosition());
    }

    gl::setUniform(locations.color       , light.color * light.intensity);
    gl::setUniform(locations.colorAmbient, light.colorAmbient);
    gl::setUniform(locations.falloffConstant , light.falloff.constant);
    gl::setUniform(locations.falloffLinear   , light.falloff.linear);
    gl::setUniform(locations.falloffQuadratic, light.falloff.quadratic);

    gl::setUniform(locations.farPlaneDistance, light.farPlaneDistance);
}

void Material::setUniformDirectionalLight(
    const BuiltinUniformLocations::DirectionalLightLocations& locations,
    const Light& light,
    const Transform& tf
) {
    if (valid(locations.direction)) {
        gl::setUniform(locations.direction, tf.getForward());
    }

    gl::setUniform(locations.color       , light.color * light.intensity);
    gl::setUniform(locations.colorAmbient, light.colorAmbient);
    gl::setUniform(locations.falloffConstant , light.falloff.constant);
    gl::setUniform(locations.falloffLinear   , light.falloff.linear);
    gl::setUniform(locations.falloffQuadratic, light.falloff.quadratic);

    if (valid(locations.lightspaceMatrix)) {
        gl::setUniform(locations.lightspaceMatrix, light.matrixPV);
    }
}

void Material::setUniformSpotLight(
    const BuiltinUniformLocations::SpotLightLocations& locations,
    const Light& light,
    const Transform& tf
) {
    if (valid(locations.position)) {
        gl::setUniform(locations.position, tf.getWorldPosition());
    }

    if (valid(locations.direction)) {
        gl::setUniform(locations.direction, glm::normalize(glm::vec3(tf.getWorldTransform()[2])));
    }

    gl::setUniform(locations.color       , light.color * light.intensity);
    gl::setUniform(locations.colorAmbient, light.colorAmbient);
    gl::setUniform(locations.falloffConstant , light.falloff.constant);
    gl::setUniform(locations.falloffLinear   , light.falloff.linear);
    gl::setUniform(locations.falloffQuadratic, light.falloff.quadratic);
    gl::setUniform(locations.innerCutoff, light.spotlightInnerCutoff);
    gl::setUniform(locations.outerCutoff, glm::min(light.spotlightInnerCutoff, light.spotlightOuterCutoff));
}
