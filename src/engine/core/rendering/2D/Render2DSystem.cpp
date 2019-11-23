//
// Created by Maksym Maisak on 2019-09-21.
//

#include "Render2DSystem.h"
#include <utility>
#include "Actor.h"
#include "TileLayer.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "Material.h"
#include "GameTime.h"
#include "Camera.h"
#include "Transform.h"
#include "Bounds.h"
#include "DebugVolumeRenderer.h"
#include "Keyboard.h"

using namespace en;

namespace {

    constexpr std::size_t MapDataTextureResolution = 256;
    // TODO load the tile layout in atlas from file
    constexpr std::size_t AtlasSize = 8;

    uint32_t pack(const Tile::AtlasCoordinates& atlasCoordinates) {

        return
            (static_cast<uint32_t>(atlasCoordinates.x) << 24) |
            (static_cast<uint32_t>(atlasCoordinates.y) << 24 >> 8);
    }

    glm::mat4 getViewMatrix(const Transform& cameraTransform) {

        const glm::mat4 viewMatrix = glm::inverse(cameraTransform.getWorldTransform());

        return utils::Keyboard::isHeld("o") ?
            glm::scale(glm::vec3(0.6f)) * viewMatrix :
            viewMatrix;
    }

    en::Bounds2DGrid getTileIndicesBounds(const Bounds2D& cameraBounds) {

        const GridPosition visibleTileIndicesMin = glm::floor(cameraBounds.min);
        const GridPosition visibleTileIndicesMax = glm::floor(cameraBounds.max);

        const glm::vec<2, std::size_t> visibleTileRangeSize = visibleTileIndicesMax - visibleTileIndicesMin;
        const glm::vec<2, std::size_t> finalTileRangeSize = glm::min(visibleTileRangeSize, MapDataTextureResolution - 1);
        const GridPosition finalTileIndicesMax = visibleTileIndicesMin + GridPosition(finalTileRangeSize);

        return {visibleTileIndicesMin, finalTileIndicesMax};
    }
}

Render2DSystem::Render2DSystem() :
    m_mapData(MapDataTextureResolution * MapDataTextureResolution),
    m_tileLayerMaterial(std::make_unique<Material>("tileLayer")),
    m_spriteShader(Resources<ShaderProgram>::get("sprite")),
    m_debugVolumeRenderer(std::make_unique<DebugVolumeRenderer>())
{
    {
        Texture::CreationSettings settings;
        settings.numMipmapLevels = 5;
        m_tileset = Textures::get(config::TEXTURE_PATH + "checkerboard.png", settings);
    }

    {
        Texture::CreationSettings settings;
        settings.internalFormat = GL_RGBA8;
        settings.wrapS = GL_CLAMP_TO_BORDER;
        settings.wrapT = GL_CLAMP_TO_BORDER;
        settings.minFilter = GL_NEAREST;
        settings.magFilter = GL_NEAREST;
        m_mapDataTexture = std::make_shared<Texture>(Texture::Size(MapDataTextureResolution), nullptr, settings);
    }

    {
        assert(m_tileLayerMaterial);
        m_tileLayerMaterial->setUniformValue("mapDataTexture", m_mapDataTexture);
        m_tileLayerMaterial->setUniformValue("tileAtlas", m_tileset);

        const glm::dvec2 atlasSizeDouble = glm::dvec2(AtlasSize);
        const glm::dvec2 dataTextureResolution = glm::dvec2(MapDataTextureResolution);
        constexpr double numPaddingPixelsPerTile = 48.0;
        m_tileLayerMaterial->setUniformValue("mapDataTextureResolution", glm::vec2(dataTextureResolution));
        m_tileLayerMaterial->setUniformValue("invMapDataTextureResolution", glm::vec2(1.0 / dataTextureResolution));
        m_tileLayerMaterial->setUniformValue("invNumTilesInAtlas", glm::vec2(1.0 / atlasSizeDouble));
        m_tileLayerMaterial->setUniformValue("paddingRelativeToTileSize",  glm::vec2((numPaddingPixelsPerTile + 1.0) * atlasSizeDouble / glm::dvec2(m_tileset->getSize())));
    }

    assert(m_mapDataTexture->isValid());
    assert(m_tileset->isValid());
}

void Render2DSystem::draw() {

    Actor cameraActor = m_engine->getMainCamera();
    if (!cameraActor) {
        return;
    }

    const Camera& camera = cameraActor.get<Camera>();
    const Transform& cameraTransform = cameraActor.get<Transform>();
    const glm::mat4 matrixView = getViewMatrix(cameraTransform);
    const glm::mat4 matrixProjection = camera.getCameraProjectionMatrix(*m_engine);

    const glm::vec2 cameraPosition = cameraTransform.getWorldPosition();
    const glm::vec2 orthographicExtents = camera.getOrthographicExtents(*m_engine);
    const Bounds2D cameraBounds = {cameraPosition - orthographicExtents, cameraPosition + orthographicExtents};

    renderLayers(cameraBounds, matrixView, matrixProjection);
    renderSprites(cameraBounds, matrixView, matrixProjection);
    m_debugVolumeRenderer->render(matrixProjection * matrixView);
}

void Render2DSystem::renderLayers(const Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection) {

    if (!m_tileset) {
        return;
    }

    glDisable(GL_DEPTH_TEST);

    const std::shared_ptr<const Mesh> quad = Mesh::getQuad();
    const Bounds2DGrid tileIndices = getTileIndicesBounds(cameraBounds);

    for (Entity e : m_registry->with<TileLayer>()) {

        updateMapDataTexture(m_registry->get<TileLayer>(e), tileIndices);

        const glm::mat4 matrixModel = glm::translate(glm::vec3(tileIndices.min, -1.f)) * glm::scale(glm::vec3(MapDataTextureResolution));
        m_tileLayerMaterial->render(quad.get(), m_engine, nullptr, matrixModel, matrixView, matrixProjection);
    }

    if (m_registry->with<TileLayer>().tryGetOne()) {

        m_debugVolumeRenderer->addAABBMinMax(glm::vec3(cameraBounds.min, 0.f), glm::vec3(cameraBounds.max, 1.f));
        m_debugVolumeRenderer->addAABBMinMax(glm::vec3(tileIndices.min, -1.f), glm::vec3(tileIndices.max + GridPosition(1), 1.f));
        m_debugVolumeRenderer->addAABB(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec4(1.f));
    }

    glEnable(GL_DEPTH_TEST);
}

void Render2DSystem::renderSprites(const Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection) {

    if (!m_spriteShader) {
        return;
    }

    const GLint textureUniformLocation = m_spriteShader->getUniformLocation("spriteTexture");
    const GLint matrixUniformLocation = m_spriteShader->getUniformLocation("matrixProjection");
    const GLint spriteColorUniformLocation = m_spriteShader->getUniformLocation("spriteColor");
    m_spriteShader->use();

    const std::shared_ptr<const Mesh> quad = Mesh::getQuad();
    const glm::mat4 matrixPV = matrixProjection * matrixView;
    for (Entity e : m_registry->with<Transform, Sprite>()) {

        const Sprite& sprite = m_registry->get<Sprite>(e);
        const Texture* const texture = sprite.texture.get();
        if (texture && texture->isValid()) {

            const glm::mat4 matrixModel = m_registry->get<Transform>(e).getWorldTransform();
            const Bounds2D spriteAABB = sprite.getAABB(matrixModel);
            if (cameraBounds.intersects(spriteAABB)) {

                gl::setUniform(textureUniformLocation, texture, 0, GL_TEXTURE_2D);
                gl::setUniform(matrixUniformLocation, matrixPV * sprite.getRenderMatrix(matrixModel));
                gl::setUniform(spriteColorUniformLocation, sprite.color);
                quad->render(0, -1, 1);
            }
        }
    }
}

void Render2DSystem::updateMapDataTexture(TileLayer& tileLayer, const en::Bounds2DGrid& tileIndices) {

    const auto tileIndexCounts = glm::vec<2, GLsizei>(tileIndices.max - tileIndices.min) + 1;
    assert(tileIndexCounts.x <= MapDataTextureResolution);
    assert(tileIndexCounts.y <= MapDataTextureResolution);
    assert(tileIndexCounts.x * tileIndexCounts.y <= m_mapData.size());

    std::size_t mapDataIndex = 0;
    for (GridCoordinate y = tileIndices.min.y; y <= tileIndices.max.y; ++y) {
        for (GridCoordinate x = tileIndices.min.x; x <= tileIndices.max.x; ++x) {
            m_mapData[mapDataIndex++] = pack(tileLayer.at({x, y}).atlasCoordinates);
        }
    }

    m_mapDataTexture->updateData2D(m_mapData.data(), GL_UNSIGNED_INT_8_8_8_8, 0, 0, tileIndexCounts.x, tileIndexCounts.y);
}
