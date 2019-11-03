//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_POSTPROCESSINGUTILITIES_H
#define ENGINE_POSTPROCESSINGUTILITIES_H

#include <utility>
#include "FramebufferBundle.h"

namespace en {

    class ShaderProgram;

    namespace PostProcessingUtilities {

        gl::FramebufferBundle makeFramebuffer(const glm::u32vec2& size);
        std::shared_ptr<ShaderProgram> getPostProcessingShader(const std::string& name);
        void renderQuad();
        void blit(const gl::TextureObject& sourceTexture, const gl::FramebufferObject& target, bool clearTarget = true);
    }
}

#endif //ENGINE_POSTPROCESSINGUTILITIES_H
