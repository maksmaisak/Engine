//
// Created by Maksym Maisak on 3/11/19.
//

#ifndef ENGINE_POSTPROCESSINGPASSTONEMAPPING_H
#define ENGINE_POSTPROCESSINGPASSTONEMAPPING_H

#include "PostProcessingPass.h"

namespace en {

    class PostProcessingPassTonemapping : public PostProcessingPass {

    public:
        PostProcessingPassTonemapping();
        void displayImGui() override;
        void render(const gl::TextureObject& sourceTexture) override;

    private:
        float m_exposure;
    };
}

#endif //ENGINE_POSTPROCESSINGPASSTONEMAPPING_H
