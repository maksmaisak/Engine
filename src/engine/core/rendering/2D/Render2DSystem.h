//
// Created by Maksym Maisak on 2019-09-21.
//

#ifndef ENGINE_RENDER2DSYSTEM_H
#define ENGINE_RENDER2DSYSTEM_H

#include <vector>
#include "System.h"
#include "Mesh.h"
#include "DebugVolumeRenderer.h"

namespace en {

    class Render2DSystem : public System {

    public:
        Render2DSystem();

        virtual void start() override;
        virtual void draw() override;

    private:

        std::unique_ptr<class Material> m_tileLayerMaterial;
        std::shared_ptr<class Texture> m_tileset;
        Mesh m_quad;

        std::vector<uint32_t> m_mapData;
        std::shared_ptr<class Texture> m_mapDataTexture;

        std::unique_ptr<DebugVolumeRenderer> m_debugVolumeRenderer;
    };
}

#endif //ENGINE_RENDER2DSYSTEM_H
