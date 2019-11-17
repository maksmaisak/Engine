//
// Created by Maksym Maisak on 2019-03-13.
//

#include "RenderSkyboxSystem.h"
#include <cassert>
#include <array>
#include "Resources.h"
#include "GLSetUniform.h"
#include "GLHelpers.h"
#include "Engine.h"
#include "Transform.h"
#include "Camera.h"
#include "ScopedBind.h"

using namespace en;

namespace {

    const std::array<float, 3 * 36> skyboxVertices {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    std::shared_ptr<Texture> getDefaultSkybox(LuaState& lua) {

        lua_getglobal(lua, "Config");
        const auto popConfig = PopperOnDestruct(lua);
        if (lua_isnil(lua, -1)) {
            return nullptr;
        }

        lua_getfield(lua, -1, "defaultSkybox");
        const auto popSkybox = PopperOnDestruct(lua);
        if (lua_isnil(lua, -1)) {
            return nullptr;
        }

        static const std::string keys[] {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back"
        };

        std::array<Name, 6> imagePaths;
        for (int i = 0; i < 6; ++i) {

            const std::optional<Name> path = lua.tryGetField<std::string>(keys[i]);
            if (!path) {
                return nullptr;
            }

            imagePaths[i] = "assets/" + path->getString();
        }

        return Resources<Texture>::get("defaultSkybox", imagePaths);
    }
}

RenderSkyboxSystem::RenderSkyboxSystem() :
    m_vao(gl::ForceCreate),
    m_buffer(gl::ForceCreate),
    m_shader(Resources<ShaderProgram>::get("skybox"))
{
    const gl::ScopedBind bindVAO(m_vao);
    const gl::ScopedBind bindBuffer(m_buffer, GL_ARRAY_BUFFER);

    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glCheckError();
}

void RenderSkyboxSystem::start()
{
    m_defaultSkybox = getDefaultSkybox(m_engine->getLuaState());
}

void RenderSkyboxSystem::draw() {

    Actor mainCamera = m_engine->getMainCamera();
    if (!mainCamera) {
        return;
    }

    Scene* scene = m_engine->getSceneManager().getCurrentScene();
    if (!scene) {
        return;
    }

    const auto& renderSettings = scene->getRenderSettings();
    if (!renderSettings.useSkybox) {
        return;
    }

    const std::shared_ptr<Texture>& skyboxTexture = renderSettings.skyboxTexture ? renderSettings.skyboxTexture : m_defaultSkybox;
    if (!skyboxTexture || !skyboxTexture->isValid() || skyboxTexture->getKind() != Texture::Kind::TextureCube) {
        return;
    }

    const glm::mat4 matrixView = glm::mat4(glm::inverse(mainCamera.get<Transform>().getWorldRotation()));

    const auto& camera = mainCamera.get<Camera>();
    const glm::mat4 matrixProjection = glm::perspective(
        glm::radians(camera.isOrthographic ? 90.f : camera.fov),
        m_engine->getWindow().getAspectRatio(),
        camera.nearPlaneDistance,
        camera.farPlaneDistance
    );

    renderSkyboxCubemap(*skyboxTexture, matrixProjection * matrixView);
}

void RenderSkyboxSystem::renderSkyboxCubemap(const Texture& cubemap, const glm::mat4& matrixPV) {

    assert(cubemap.isValid() && cubemap.getKind() == Texture::Kind::TextureCube);
    assert(m_shader);

    glDepthFunc(GL_LEQUAL);

    {
        const gl::ScopedBind bindVAO(m_vao);

        m_shader->use();
        gl::setUniform(m_shader->getUniformLocation("matrixPV"), matrixPV);
        gl::setUniform(m_shader->getUniformLocation("skyboxTexture"), &cubemap, 0, GL_TEXTURE_CUBE_MAP);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glDepthFunc(GL_LESS);
}
