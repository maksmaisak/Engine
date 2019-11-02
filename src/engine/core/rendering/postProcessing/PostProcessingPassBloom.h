//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_POSTPROCESSINGPASSBLOOM_H
#define ENGINE_POSTPROCESSINGPASSBLOOM_H

#include "TextureObject.h"
#include "FramebufferBundle.h"
#include "PostProcessingUtilities.h"

namespace en {

    class PostProcessingPassBloom {

    public:
        PostProcessingPassBloom();
        void render(const gl::TextureObject& sourceTexture);

    private:
        static void renderQuad();

        gl::FramebufferBundle m_intermediateFramebuffer;
    };
}

#endif //ENGINE_POSTPROCESSINGPASSBLOOM_H
