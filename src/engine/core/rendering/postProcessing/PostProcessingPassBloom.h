//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_POSTPROCESSINGPASSBLOOM_H
#define ENGINE_POSTPROCESSINGPASSBLOOM_H

#include "TextureObject.h"
#include "FramebufferBundle.h"

namespace en {

    struct BloomSettings {

        float brightnessThreshold = 0.8f;

        float blurStandardDeviation = 4.f;
        int blurKernelSize = 24;
        int numBlurIterations = 1;

        float intensity = 1.f;
    };

    class PostProcessingPassBloom {

    public:
        PostProcessingPassBloom();
        void render(const gl::TextureObject& sourceTexture);

        BloomSettings m_settings;

    private:
        void updateSettings();
        void blur();
        void bloomCombine(const gl::TextureObject& originalTexture, const gl::TextureObject& blurredTexture);

        gl::FramebufferBundle m_blurFramebuffers[2];
    };
}

#endif //ENGINE_POSTPROCESSINGPASSBLOOM_H
