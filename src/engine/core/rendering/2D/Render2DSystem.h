//
// Created by Maksym Maisak on 2019-09-21.
//

#ifndef ENGINE_RENDER2DSYSTEM_H
#define ENGINE_RENDER2DSYSTEM_H

#include <vector>
#include "System.h"
#include "Mesh.h"
#include "DebugVolumeRenderer.h"
#include "glm.h"

namespace en {

    class Render2DSystem : public System {

    public:
        Render2DSystem();

        void start() override;
        void draw() override;

    private:

        void renderLayers(const glm::vec2& cameraCenter, const glm::vec2& orthographicHalfSize, const glm::mat4& matrixView, const glm::mat4& matrixProjection);
        void renderSprites(const glm::mat4& matrixView, const glm::mat4& matrixProjection);

        std::unique_ptr<class Material> m_tileLayerMaterial;
        std::shared_ptr<class Texture> m_tileset;

        std::vector<uint32_t> m_mapData;
        std::shared_ptr<class Texture> m_mapDataTexture;
        std::shared_ptr<class ShaderProgram> m_spriteShader;

        std::unique_ptr<DebugVolumeRenderer> m_debugVolumeRenderer;
    };
}

#endif //ENGINE_RENDER2DSYSTEM_H
