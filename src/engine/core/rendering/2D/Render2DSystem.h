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
#include "Grid.h"
#include "Material.h"

namespace en {

    class Render2DSystem : public System {

    public:
        Render2DSystem();
        void draw() override;

    private:
        void renderLayers(const Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void renderSprites(const Bounds2D& cameraBounds, const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void updateMapDataTexture(class TileLayer& tileLayer, const Bounds2DGrid& tileIndices);

        std::unique_ptr<Material> m_tileLayerMaterial;
        std::shared_ptr<class Texture> m_tileset;

        std::vector<uint32_t> m_mapData;
        std::shared_ptr<class Texture> m_mapDataTexture;
        std::shared_ptr<class ShaderProgram> m_spriteShader;

        std::unique_ptr<DebugVolumeRenderer> m_debugVolumeRenderer;
    };
}

#endif //ENGINE_RENDER2DSYSTEM_H
