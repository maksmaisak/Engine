//
// Created by Maksym Maisak on 2019-09-21.
//

#ifndef ENGINE_RENDER2DSYSTEM_H
#define ENGINE_RENDER2DSYSTEM_H

#include <vector>
#include "Bounds.h"
#include "System.h"
#include "Mesh.h"
#include "DebugVolumeRenderer.h"
#include "glm.h"
#include "TileLayer.h"

namespace en {

    class Render2DSystem : public System {

    public:
        Render2DSystem();
        void draw() override;

    private:
        void renderLayers(const utils::Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void renderSprites(const utils::Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void updateMapDataTexture(TileLayer& tileLayer, const TileLayer::Coordinates& min, const TileLayer::Coordinates& max);

        std::unique_ptr<class Material> m_tileLayerMaterial;
        std::shared_ptr<class Texture> m_tileset;

        std::vector<uint32_t> m_mapData;
        std::shared_ptr<class Texture> m_mapDataTexture;
        std::shared_ptr<class ShaderProgram> m_spriteShader;

        std::unique_ptr<DebugVolumeRenderer> m_debugVolumeRenderer;
    };
}

#endif //ENGINE_RENDER2DSYSTEM_H
