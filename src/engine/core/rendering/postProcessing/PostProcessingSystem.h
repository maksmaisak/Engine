//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_POSTPROCESSINGSYSTEM_H
#define ENGINE_POSTPROCESSINGSYSTEM_H

#include "System.h"
#include "TextureObject.h"

namespace en {

    class PostProcessingSystem : public System {

    public:
        explicit PostProcessingSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void start() override;
        void draw() override;

    private:
        std::shared_ptr<RenderingSharedState> m_renderingSharedState;
        gl::TextureObject m_prePostProcessRenderTexture;
    };
}

#endif //ENGINE_POSTPROCESSINGSYSTEM_H
