//
// Created by Maksym Maisak on 4/10/19.
//

#include "Render3DSystem.h"
#include "RenderingSharedState.h"
#include "Transform.h"
#include "Camera.h"
#include "RenderInfo.h"
#include "Material.h"
#include "Name.h"

using namespace en;

namespace {

    inline void checkRenderingError(const Actor& actor) {

        if (glCheckError() != GL_NO_ERROR) {

            const auto* const namePtr = actor.tryGet<Name>();
            const std::string name = namePtr ? *namePtr : "unnamed";
            std::cerr << "Error while rendering " << name << std::endl;
        }
    }
}

Render3DSystem::Render3DSystem(std::shared_ptr<RenderingSharedState> renderingSharedState) :
    m_renderingSharedState(std::move(renderingSharedState))
{
    assert(m_renderingSharedState);
}

void Render3DSystem::start() {

    m_shadowMapper = std::make_unique<ShadowMapper>(*m_engine, m_renderingSharedState);
}

void Render3DSystem::draw() {

    if (m_renderingSharedState->enableStaticBatching) {
        updateBatches();
    }

    m_shadowMapper->updateDepthMaps();
    render();
}

void Render3DSystem::updateBatches() {

    const auto findBatchMesh = [&batches = m_renderingSharedState->batches](const std::shared_ptr<Material>& material) -> Mesh& {

        const auto foundIt = batches.find(material);
        if (foundIt != batches.end()) {
            return foundIt->second;
        }

        const auto [it, didEmplace] = batches.emplace(std::make_pair(material, Mesh{}));
        assert(didEmplace);
        return it->second;
    };

    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (renderInfo.isInBatch || !renderInfo.isBatchingStatic || !renderInfo.material || !renderInfo.model) {
            continue;
        }

        Mesh& batchMesh = findBatchMesh(renderInfo.material);
        const glm::mat4& worldMatrix = m_registry->get<Transform>(e).getWorldTransform();
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            batchMesh.add(mesh, worldMatrix);
        }

        renderInfo.isInBatch = true;
    }

    for (auto& [material, batch] : m_renderingSharedState->batches) {
        batch.updateBuffers();
    }
}

void Render3DSystem::render() {

    if (Actor mainCamera = m_engine->getMainCamera()) {

        m_engine->getWindow().setViewport();

        const glm::mat4 matrixView = glm::inverse(mainCamera.get<Transform>().getWorldTransform());
        const glm::mat4 matrixProjection = mainCamera.get<Camera>().getCameraProjectionMatrix(*m_engine);
        renderBatches(matrixView, matrixProjection);
        renderEntities(matrixView, matrixProjection);
    }
}

void Render3DSystem::renderBatches(const glm::mat4& matrixView, const glm::mat4& matrixProjection) {

    for (const auto& [material, mesh] : m_renderingSharedState->batches) {
        material->render(&mesh, m_engine, &m_renderingSharedState->depthMaps, glm::mat4(1), matrixView, matrixProjection);
    }
}

void Render3DSystem::renderEntities(const glm::mat4& matrixView, const glm::mat4& matrixProjection) {

    auto useMaterial = [&, previousMaterial = (Material*)nullptr](Material* material, const glm::mat4& matrixModel) mutable {

        if (previousMaterial == material) {
            material->updateModelMatrix(matrixModel);
        } else {
            material->use(m_engine, &m_renderingSharedState->depthMaps, matrixModel, matrixView, matrixProjection);
        }

        previousMaterial = material;
    };

    std::size_t numDrawCallsSavedByBatching = 0;
    for (Entity e : m_registry->with<Transform, RenderInfo>()) {

        const auto& renderInfo = m_registry->get<RenderInfo>(e);
        if (!renderInfo.isValidForRendering()) {
            continue;
        }

        if (renderInfo.isInBatch) {
            numDrawCallsSavedByBatching += renderInfo.model->getMeshes().size();
            continue;
        }

        Material* const material = renderInfo.material.get();
        useMaterial(material, m_registry->get<Transform>(e).getWorldTransform());
        for (const Mesh& mesh : renderInfo.model->getMeshes()) {
            material->setAttributesAndDraw(&mesh);
        }

        checkRenderingError(m_engine->actor(e));
    }
}
