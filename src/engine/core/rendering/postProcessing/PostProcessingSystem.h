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
#include "Receiver.h"

namespace en {

    class PostProcessingSystem : public System, Receiver<sf::Event> {

    public:
        explicit PostProcessingSystem(std::shared_ptr<struct RenderingSharedState> renderingSharedState);
        void start() override;
        void draw() override;

    private:
        void receive(const sf::Event& event) override;
        void setUp(const glm::u32vec2& size);

        std::shared_ptr<RenderingSharedState> m_renderingSharedState;
        std::vector<std::unique_ptr<PostProcessingPass>> m_postProcessingPasses;
    };
}

#endif //ENGINE_POSTPROCESSINGSYSTEM_H
