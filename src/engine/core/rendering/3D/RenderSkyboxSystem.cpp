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

        static std::string keys[] = {
            "right",
            "left",
            "top",
            "bottom",
            "front",
            "back"
        };

        std::array<std::string, 6> imagePaths;
        for (int i = 0; i < 6; ++i) {

            std::optional<std::string> path = lua.tryGetField<std::string>(keys[i]);
            if (!path) {
                return nullptr;
            }

            imagePaths[i] = "assets/" + *path;
        }

        return Resources<Texture>::get("defaultSkybox", imagePaths);
    }
}

RenderSkyboxSystem::RenderSkyboxSystem() :
    m_shader(Resources<ShaderProgram>::get("skybox"))
{
    glGenBuffers(1, &m_bufferId);

    // Set up the VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
        glCheckError();
        {
            glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);
            glCheckError();
            glEnableVertexAttribArray(0);
            glCheckError();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glCheckError();
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glCheckError();
    }
    glBindVertexArray(0);

    glCheckError();
}

RenderSkyboxSystem::~RenderSkyboxSystem() {

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_bufferId);
}

void RenderSkyboxSystem::start()
{
    m_defaultSkybox = getDefaultSkybox(m_engine->getLuaState());
}

void RenderSkyboxSystem::draw() {

    Actor mainCamera = m_engine->actor(m_registry->with<Transform, Camera>().tryGetOne());
    if (!mainCamera)
        return;

    Scene* scene = m_engine->getSceneManager().getCurrentScene();
    if (!scene)
        return;

    const auto& renderSettings = scene->getRenderSettings();
    if (!renderSettings.useSkybox)
        return;

    const std::shared_ptr<Texture>& skyboxTexture = renderSettings.skyboxTexture ? renderSettings.skyboxTexture : m_defaultSkybox;
    if (!skyboxTexture || !skyboxTexture->isValid() || skyboxTexture->getKind() != Texture::Kind::TextureCube)
        return;

    const glm::mat4 matrixView = glm::mat4(glm::inverse(mainCamera.get<Transform>().getWorldRotation()));

    const auto size = m_engine->getWindow().getSize();
    const float aspectRatio = (float)size.x / size.y;
    const auto& camera = mainCamera.get<Camera>();
    const glm::mat4 matrixProjection = glm::perspective(
        glm::radians(camera.isOrthographic ? 90.f : camera.fov),
        aspectRatio,
        camera.nearPlaneDistance,
        camera.farPlaneDistance
    );

    renderSkyboxCubemap(*skyboxTexture, matrixProjection * matrixView);
}

void RenderSkyboxSystem::renderSkyboxCubemap(const Texture& cubemap, const glm::mat4& matrixPV) {

    assert(cubemap.isValid() && cubemap.getKind() == Texture::Kind::TextureCube);
    assert(m_shader);

    glDepthFunc(GL_LEQUAL);

    m_shader->use();
    glCheckError();
    glBindVertexArray(m_vao);
    {
        gl::setUniform(m_shader->getUniformLocation("matrixPV"), matrixPV);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getId());
        glUniform1i(m_shader->getUniformLocation("skyboxTexture"), 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glCheckError();
    }
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}
