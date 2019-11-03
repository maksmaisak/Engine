//
// Created by Maksym Maisak on 3/11/19.
//

#ifndef ENGINE_POSTPROCESSINGPASSTONEMAPPING_H
#define ENGINE_POSTPROCESSINGPASSTONEMAPPING_H

#include "PostProcessingPass.h"

namespace en {

    class PostProcessingPassTonemapping : public PostProcessingPass {

    public:
        void render(const gl::TextureObject& sourceTexture) override;
    };
}

#endif //ENGINE_POSTPROCESSINGPASSTONEMAPPING_H
