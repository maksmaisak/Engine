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

using namespace en;

static constexpr uint32_t MapDataTextureResolution = 40;

Render2DSystem::Render2DSystem() :
    m_quad(Mesh::makeQuad()),
    m_mapData(MapDataTextureResolution * MapDataTextureResolution),
    m_tileset(Textures::get(config::TEXTURE_PATH + "TileSet.png")),
    m_tileLayerMaterial(std::make_unique<Material>("tileLayer"))
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
    const glm::vec<2, int64_t> visibleTileIndicesMin = glm::floor(viewBottomLeftCornerPosition) - 1.f;
    const glm::vec<2, int64_t> visibleTileIndicesMax = glm::ceil(cameraCenter + orthographicHalfSize);
    const glm::vec<2, int64_t> visibleTileRangeSize = visibleTileIndicesMax - visibleTileIndicesMin;

    const glm::mat4 matrixView = glm::inverse(cameraTransform.getWorldTransform());//glm::inverse(glm::mat3(cameraTransform.getWorldTransform()));
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

                    m_mapData[x + y * MapDataTextureResolution] =
                        (((uint32_t)glm::abs(indices.x)) << 24) | (((uint32_t)glm::abs(indices.y)) << 24 >> 8);
                        //((uint32_t)glm::floor(255.f * (float)indices.x / MapDataTextureResolution) << 24) |
                        //((uint32_t)glm::floor(255.f * (float)indices.y / MapDataTextureResolution) << 24 >> 8);
                        //0x03030000; //tileLayer.getTileInfoAt({x, y});
                }
            }
        }
        m_mapDataTexture->updateData2D(m_mapData.data(), GL_UNSIGNED_INT_8_8_8_8);

        const glm::mat4 matrixModel = glm::translate(glm::vec3(visibleTileIndicesMin, 0.f)) * glm::scale(glm::vec3(MapDataTextureResolution));

        m_tileLayerMaterial->render(&m_quad, m_engine, nullptr, matrixModel, matrixView, matrixProjection);
    }

    glEnable(GL_DEPTH_TEST);
}
