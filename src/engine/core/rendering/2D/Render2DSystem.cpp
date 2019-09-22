//
// Created by Maksym Maisak on 2019-09-21.
//

#include "Render2DSystem.h"
#include "Actor.h"
#include "TileLayer.h"
#include "Mesh.h"
#include "Texture.h"
#include "Resources.h"
#include "Material.h"
#include "GameTime.h"
#include "Camera.h"
#include "Transform.h"
#include "Bounds.h"
#include "DebugVolumeRenderer.h"
#include "KeyboardHelper.h"

using namespace en;

static constexpr uint32_t MapDataTextureResolution = 256;

Render2DSystem::Render2DSystem() :
    m_quad(Mesh::makeQuad()),
    m_mapData(MapDataTextureResolution * MapDataTextureResolution),
    m_tileset(Textures::get(config::TEXTURE_PATH + "TileSet.png")),
    m_tileLayerMaterial(std::make_unique<Material>("tileLayer")),
    m_debugVolumeRenderer(std::make_unique<DebugVolumeRenderer>())
{
    Texture::CreationSettings settings;
    settings.internalFormat = GL_RGBA;
    settings.wrapS = GL_CLAMP_TO_EDGE;
    settings.wrapT = GL_CLAMP_TO_EDGE;
    settings.minFilter = GL_NEAREST;
    settings.magFilter = GL_NEAREST;
    m_mapDataTexture = std::make_shared<Texture>(glm::vec2(MapDataTextureResolution), settings);

    m_tileLayerMaterial->setUniformValue("mapDataTexture", m_mapDataTexture);
    m_tileLayerMaterial->setUniformValue("tileAtlas", m_tileset);

    assert(m_mapDataTexture->isValid());
    assert(m_tileset->isValid());
}

void Render2DSystem::start() {

    if (!m_registry->with<TileLayer>().tryGetOne()) {

        m_engine->makeActor("Test tile layer").add<TileLayer>();
    }

    if (!m_registry->with<Camera, Transform>().tryGetOne()) {

        Actor cameraActor = m_engine->makeActor("Test camera");
        cameraActor.add<Camera>().isOrthographic = true;
        cameraActor.add<Transform>().move({0, 0, 10.f});
    }
}

void Render2DSystem::draw() {

    if (!m_tileset)
        return;

    Actor cameraActor = m_engine->actor(m_registry->with<Camera, Transform>().tryGetOne());
    if (!cameraActor)
        return;

    glDisable(GL_DEPTH_TEST);

    const Camera& camera = cameraActor.get<Camera>();
    const Transform& cameraTransform = cameraActor.get<Transform>();

    const glm::vec2 cameraCenter = cameraTransform.getWorldPosition();
    const glm::vec2 orthographicHalfSize = camera.getOrthographicExtents(*m_engine);

    const glm::vec2 viewBottomLeftCornerPosition = cameraCenter - orthographicHalfSize;
    const glm::vec<2, int64_t> visibleTileIndicesMin = glm::floor(viewBottomLeftCornerPosition);
    const glm::vec<2, int64_t> visibleTileIndicesMax = glm::ceil(cameraCenter + orthographicHalfSize);
    const glm::vec<2, int64_t> visibleTileRangeSize = visibleTileIndicesMax - visibleTileIndicesMin;

    const glm::mat4 matrixView = (utils::KeyboardHelper::isHeld("o") ? glm::scale(glm::vec3(0.6f)) : glm::mat4(1.f)) * glm::inverse(cameraTransform.getWorldTransform());
    const glm::mat4 matrixProjection = cameraActor.get<Camera>().getCameraProjectionMatrix(*m_engine);

    for (Entity e : m_registry->with<TileLayer>()) {

        const auto& tileLayer = m_registry->get<TileLayer>(e);

        for (int64_t x = 0; x < visibleTileRangeSize.x; ++x) {
            for (int64_t y = 0; y < visibleTileRangeSize.y; ++y) {
                if (x < MapDataTextureResolution && y < MapDataTextureResolution) {

                    glm::vec<2, int64_t> indices = {
                        visibleTileIndicesMin.x + x,
                        visibleTileIndicesMin.y + y
                    };

                    const uint32_t uintX = indices.x >= 0 ? indices.x % 11 : glm::abs(11 + indices.x % 11);
                    const uint32_t uintY = indices.y >= 0 ? indices.y % 6  : glm::abs(6  + indices.y % 6 );
                    m_mapData[x + y * MapDataTextureResolution] = (uintX << 24) | (uintY << 24 >> 8);
                }
            }
        }
        m_mapDataTexture->updateData2D(m_mapData.data(), GL_UNSIGNED_INT_8_8_8_8);

        const glm::mat4 matrixModel = glm::translate(glm::vec3(visibleTileIndicesMin, 0.f)) * glm::scale(glm::vec3(MapDataTextureResolution));

        m_tileLayerMaterial->render(&m_quad, m_engine, nullptr, matrixModel, matrixView, matrixProjection);
    }

    m_debugVolumeRenderer->addAABB(glm::vec3(cameraCenter, 0.f), glm::vec3(orthographicHalfSize, 1.f));
    m_debugVolumeRenderer->addAABBMinMax(glm::vec3(visibleTileIndicesMin, -1.f), glm::vec3(visibleTileIndicesMax,  1.f));

    m_debugVolumeRenderer->addAABB(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec4(1.f));
    m_debugVolumeRenderer->render(matrixProjection * matrixView);

    glEnable(GL_DEPTH_TEST);
}
