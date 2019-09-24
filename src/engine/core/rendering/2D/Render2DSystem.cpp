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

namespace {

    constexpr std::size_t MapDataTextureResolution = 256;
    // TODO load the tile layout in atlas from file
    constexpr std::size_t AtlasSize = 8;

    uint32_t pack(const Tile::AtlasCoordinates& atlasCoordinates) {
        return ((uint32_t)atlasCoordinates.x << 24) | ((uint32_t)atlasCoordinates.y << 24 >> 8);
    }
}

Render2DSystem::Render2DSystem() :
    m_mapData(MapDataTextureResolution * MapDataTextureResolution),
    m_tileLayerMaterial(std::make_unique<Material>("tileLayer")),
    m_debugVolumeRenderer(std::make_unique<DebugVolumeRenderer>())
{
    {
        Texture::CreationSettings settings;
        settings.numMipmapLevels = 5;
        m_tileset = Textures::get(config::TEXTURE_PATH + "checkerboard.png", settings);
    }

    {
        Texture::CreationSettings settings;
        settings.internalFormat = GL_RGBA;
        settings.wrapS = GL_CLAMP_TO_BORDER;
        settings.wrapT = GL_CLAMP_TO_BORDER;
        settings.minFilter = GL_NEAREST;
        settings.magFilter = GL_NEAREST;
        m_mapDataTexture = std::make_shared<Texture>(Texture::Size(MapDataTextureResolution), settings);
    }

    {
        assert(m_tileLayerMaterial);
        const auto& m = *m_tileLayerMaterial;
        m_tileLayerMaterial->setUniformValue("mapDataTexture", m_mapDataTexture);
        m_tileLayerMaterial->setUniformValue("tileAtlas", m_tileset);

        using vec2D = glm::vec<2, double>;
        const vec2D atlasSizeDouble = vec2D(AtlasSize);
        const vec2D dataTextureResolution = vec2D(MapDataTextureResolution);

        const double numPaddingPixelsPerTile = 48.0;
        m_tileLayerMaterial->setUniformValue("mapDataTextureResolution", glm::vec2(dataTextureResolution));
        m_tileLayerMaterial->setUniformValue("invMapDataTextureResolution", glm::vec2(1.0 / dataTextureResolution));
        m_tileLayerMaterial->setUniformValue("invNumTilesInAtlas", glm::vec2(1.0 / atlasSizeDouble));
        m_tileLayerMaterial->setUniformValue("paddingRelativeToTileSize",  glm::vec2((numPaddingPixelsPerTile + 1.0) * atlasSizeDouble / vec2D(m_tileset->getSize())));
    }

    assert(m_mapDataTexture->isValid());
    assert(m_tileset->isValid());
}

void Render2DSystem::start() {

    if (!m_registry->with<TileLayer>().tryGetOne()) {

        TileLayer& layer = m_engine->makeActor("Test tile layer").add<TileLayer>();

        const float radius = 20.f;
        for (float theta = 0.f; theta < glm::two_pi<float>(); theta += 0.001f) {

            const TileLayer::Coordinates position = {
                glm::cos(theta) * radius,
                glm::sin(theta) * radius
            };

            layer.at(position).atlasCoordinates = {7, 7};
        }
    }

    if (!m_registry->with<Camera, Transform>().tryGetOne()) {

        Actor cameraActor = m_engine->makeActor("Test camera");
        cameraActor.add<Camera>().isOrthographic = true;
        cameraActor.add<Transform>().move({0, 0, 10.f});
    }
}

void Render2DSystem::draw() {

    if (!m_tileset) {
        return;
    }

    Actor cameraActor = m_engine->actor(m_registry->with<Camera, Transform>().tryGetOne());
    if (!cameraActor) {
        return;
    }

    glDisable(GL_DEPTH_TEST);

    const Camera& camera = cameraActor.get<Camera>();
    const Transform& cameraTransform = cameraActor.get<Transform>();

    const glm::vec2 cameraCenter = cameraTransform.getWorldPosition();
    const glm::vec2 orthographicHalfSize = camera.getOrthographicExtents(*m_engine);

    const glm::vec2 viewBottomLeftCornerPosition = cameraCenter - orthographicHalfSize;
    const glm::vec<2, int64_t> visibleTileIndicesMin = glm::floor(viewBottomLeftCornerPosition);
    const glm::vec<2, int64_t> visibleTileIndicesMax = glm::ceil(cameraCenter + orthographicHalfSize);
    const glm::vec<2, std::size_t> visibleTileRangeSize = visibleTileIndicesMax - visibleTileIndicesMin;

    const glm::mat4 matrixView = (utils::KeyboardHelper::isHeld("o") ? glm::scale(glm::vec3(0.6f)) : glm::mat4(1.f)) * glm::inverse(cameraTransform.getWorldTransform());
    const glm::mat4 matrixProjection = cameraActor.get<Camera>().getCameraProjectionMatrix(*m_engine);

    for (Entity e : m_registry->with<TileLayer>()) {

        auto& tileLayer = m_registry->get<TileLayer>(e);

        std::size_t mapDataIndex = 0;
        for (std::size_t y = 0; y < visibleTileRangeSize.y; ++y) {
            for (std::size_t x = 0; x < visibleTileRangeSize.x; ++x) {
                if (x < MapDataTextureResolution && y < MapDataTextureResolution) {

                    const Tile& tile = tileLayer.at({visibleTileIndicesMin.x + x, visibleTileIndicesMin.y + y});
                    m_mapData[mapDataIndex++] = pack(tile.atlasCoordinates);
                }
            }
        }
        m_mapDataTexture->updateData2D(m_mapData.data(), GL_UNSIGNED_INT_8_8_8_8, 0, 0, glm::min(visibleTileRangeSize.x, MapDataTextureResolution), glm::min(visibleTileRangeSize.y, MapDataTextureResolution));

        const glm::mat4 matrixModel = glm::translate(glm::vec3(visibleTileIndicesMin, 0.f)) * glm::scale(glm::vec3(MapDataTextureResolution));
        m_tileLayerMaterial->render(Mesh::getQuad().get(), m_engine, nullptr, matrixModel, matrixView, matrixProjection);
    }

    m_debugVolumeRenderer->addAABB(glm::vec3(cameraCenter, 0.f), glm::vec3(orthographicHalfSize, 1.f));
    m_debugVolumeRenderer->addAABBMinMax(glm::vec3(visibleTileIndicesMin, -1.f), glm::vec3(visibleTileIndicesMax,  1.f));
    m_debugVolumeRenderer->addAABB(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec4(1.f));
    m_debugVolumeRenderer->render(matrixProjection * matrixView);

    glEnable(GL_DEPTH_TEST);
}
