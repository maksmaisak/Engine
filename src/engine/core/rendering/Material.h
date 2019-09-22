//
// Created by Maksym Maisak on 2019-01-09.
//

#ifndef ENGINE_MATERIAL_H
#define ENGINE_MATERIAL_H

#include <memory>
#include <string>
#include <unordered_map>
#include <tuple>
#include "glm.h"
#include "ShaderProgram.hpp"
#include "Texture.h"
#include "TupleUtils.h"
#include "Light.h"
#include "Transform.h"
#include "Exception.h"
#include "SFML/Graphics.hpp"

namespace en {

    class LuaState;
    class DepthMaps;

    /// A generic material that works with a given shader.
    /// Automatically sets `built-in` uniforms like transformation matrices, time and lighting data.
    /// Use material.setUniformValue to set material-specific values for uniforms other than the built-in ones.
    /// The material will set those when it's time to render.
    class Material final {

    public:

        explicit Material(const std::string& shaderFilename);
        explicit Material(std::shared_ptr<ShaderProgram> shader);
        /// Makes a material from the table on top of stack in the given lua state
        explicit Material(LuaState& lua);

        void use(
             Engine* engine,
             DepthMaps* depthMaps,
             const glm::mat4& modelMatrix,
             const glm::mat4& viewMatrix,
             const glm::mat4& projectionMatrix
        );

        void setAttributesAndDraw(const class Mesh* mesh);

        /// Sets the built-in uniforms related to the model matrix.
        /// Useful if you're rendering multiple meshes with the same material and want to avoid unnecessary state changes.
        /// Only call this if the material is in use.
        void updateModelMatrix(const glm::mat4& modelMatrix);

        /// A shortcut for `use` followed by `setAttributesAndDraw`
        void render(
            const class Mesh* mesh,
            Engine* engine,
            DepthMaps* depthMaps,
            const glm::mat4& modelMatrix,
            const glm::mat4& viewMatrix,
            const glm::mat4& projectionMatrix
        );

        template<typename T>
        void setUniformValue(const std::string& name, const T& value);

        template<typename T>
        const T& getUniformValue(const std::string& name);

        // Information necessary to use a font atlas as a texture uniform.
        // Pass instances of this into setUniformValue<FontAtlas>
        struct FontAtlas {
            std::shared_ptr<sf::Font> font;
            unsigned int characterSize = 30;
        };

    private:

        template<typename T>
        using LocationToUniformValue = std::unordered_map<GLint, T>;

        template<typename... T>
        using UniformValues = std::tuple<LocationToUniformValue<T>...>;

        static constexpr int MAX_NUM_POINT_LIGHTS       = 10;
        static constexpr int MAX_NUM_DIRECTIONAL_LIGHTS = 10;
        static constexpr int MAX_NUM_SPOT_LIGHTS        = 10;

        std::shared_ptr<ShaderProgram> m_shader;

        int m_numSupportedPointLights = 0;
        int m_numSupportedDirectionalLights = 0;
        int m_numSupportedSpotLights = 0;

        struct BuiltinUniformLocations {

            GLint model       = -1;
            GLint view        = -1;
            GLint projection  = -1;
            GLint pvm         = -1;
            GLint modelNormal = -1;

            GLint time = -1;

            GLint viewPosition = -1;

            GLint ambientColor = -1;

            GLint depthCubemaps = -1;
            GLint directionalDepthMaps = -1;

            GLint numPointLights = -1;
            struct PointLightLocations {

                GLint position = -1;

                GLint color        = -1;
                GLint colorAmbient = -1;

                GLint falloffConstant  = -1;
                GLint falloffLinear    = -1;
                GLint falloffQuadratic = -1;

                GLint farPlaneDistance = -1;

            } pointLights[MAX_NUM_POINT_LIGHTS];

            GLint numDirectionalLights = -1;
            struct DirectionalLightLocations {

                GLint direction = -1;

                GLint color        = -1;
                GLint colorAmbient = -1;

                GLint falloffConstant  = -1;
                GLint falloffLinear    = -1;
                GLint falloffQuadratic = -1;

                GLint lightspaceMatrix = -1;

            } directionalLights[MAX_NUM_DIRECTIONAL_LIGHTS];

            GLint numSpotLights = -1;
            struct SpotLightLocations {

                GLint position  = -1;
                GLint direction = -1;

                GLint color        = -1;
                GLint colorAmbient = -1;

                GLint falloffConstant  = -1;
                GLint falloffLinear    = -1;
                GLint falloffQuadratic = -1;
                GLint innerCutoff = -1;
                GLint outerCutoff = -1;

            } spotLights[MAX_NUM_SPOT_LIGHTS];;

        } m_builtinUniformLocations;

        struct AttributeLocations {

            GLint vertex    = -1;
            GLint normal    = -1;
            GLint uv        = -1;
            GLint tangent   = -1;
            GLint bitangent = -1;

        } m_attributeLocations;

        // All uniforms in the shader.
        std::unordered_map<std::string, UniformInfo> m_uniforms;

        // Values of custom material-specific uniforms.
        // A tuple of maps float location to value.
        // Only types listed here will be supported as custom uniform values,
        // i.e settable via material.setUniformValue
        UniformValues<
            GLint, GLuint, GLfloat,
            glm::vec2, glm::vec3, glm::vec4,
            glm::mat4,
            std::shared_ptr<Texture>,
            FontAtlas
        > m_uniformValues;

        GLenum m_numTexturesInUse = 0;

        void detectAllUniforms();
        BuiltinUniformLocations cacheBuiltinUniformLocations();
        AttributeLocations cacheAttributeLocations();

        void setBuiltinUniforms(Engine* engine, DepthMaps* depthMaps, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
        void setCustomUniforms();

        template<typename T>
        void setCustomUniformsOfType(const LocationToUniformValue<T>& values);

        bool setUniformTexture(GLint uniformLocation, GLuint textureId, GLenum textureTarget = GL_TEXTURE_2D);

        void setUniformsPointLight     (const BuiltinUniformLocations::PointLightLocations&       locations, const Light& light, const Transform& tf);
        void setUniformDirectionalLight(const BuiltinUniformLocations::DirectionalLightLocations& locations, const Light& light, const Transform& tf);
        void setUniformSpotLight       (const BuiltinUniformLocations::SpotLightLocations&        locations, const Light& light, const Transform& tf);
    };

    template<typename T>
    inline void Material::setUniformValue(const std::string& name, const T& value) {

        const auto it = m_uniforms.find(name);
        if (it == m_uniforms.end()) {
            throw utils::Exception("No such uniform: " + name);
        }

        static_assert(
            has_type_v<LocationToUniformValue<T>, decltype(m_uniformValues)>,
            "This type is unsupported for custom uniforms."
        );

        // TODO Type checking for custom uniforms.
        auto& values = std::get<LocationToUniformValue<T>>(m_uniformValues);
        values[it->second.location] = value;
    }

    template<typename T>
    const T& Material::getUniformValue(const std::string& name) {

        const auto it = m_uniforms.find(name);
        if (it == m_uniforms.end()) {
            throw utils::Exception("No such uniform: " + name);
        }

        static_assert(
            has_type_v<LocationToUniformValue<T>, decltype(m_uniformValues)>,
            "This type is unsupported for custom uniforms."
        );

        auto& values = std::get<LocationToUniformValue<T>>(m_uniformValues);
        return values[it->second.location];
    }
}

#endif //ENGINE_MATERIAL_H
