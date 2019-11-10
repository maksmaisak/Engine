//
// Created by Maksym Maisak on 2/11/19.
//

#ifndef ENGINE_POSTPROCESSINGSYSTEM_H
#define ENGINE_POSTPROCESSINGSYSTEM_H

#include "System.h"
#include <vector>
#include <memory>
#include "TextureObject.h"
#include "PostProcessingPass.h"
#include "FramebufferBundle.h"
#include "Receiver.h"
#include "Window.h"

namespace en {

    class PostProcessingSystem : public System, Receiver<Window::FramebufferSizeEvent> {

    public:
        explicit PostProcessingSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void start() override;
        void draw() override;

    private:
        void receive(const Window::FramebufferSizeEvent& event) override;
        void setUp(const glm::u32vec2& size);

        std::shared_ptr<RenderingSharedState> m_renderingSharedState;
        gl::FramebufferBundle m_intermediateBuffers[2];
        std::vector<std::unique_ptr<PostProcessingPass>> m_postProcessingPasses;
    };
}

#endif //ENGINE_POSTPROCESSINGSYSTEM_H
