//
// Created by Maksym Maisak on 3/11/19.
//

#ifndef ENGINE_POSTPROCESSINGPASS_H
#define ENGINE_POSTPROCESSINGPASS_H

#include "glm.h"

namespace gl {
    class TextureObject;
}

namespace en {

    class PostProcessingPass {

    public:
        virtual ~PostProcessingPass() = default;

        /// Set up framebuffers with the given size in pixels.
        inline virtual void setUp(const glm::u32vec2& size) {};

        /// Render data processed from the given texture to the currently bound framebuffer.
        virtual void render(const gl::TextureObject& sourceTexture) = 0;
    };
}

#endif //ENGINE_POSTPROCESSINGPASS_H
