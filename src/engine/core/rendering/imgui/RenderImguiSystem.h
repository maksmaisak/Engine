//
// Created by Maksym Maisak on 27/10/19.
//

#ifndef ENGINE_RENDERIMGUISYSTEM_H
#define ENGINE_RENDERIMGUISYSTEM_H

#include "System.h"
#include "Receiver.h"

namespace en {

    class RenderImguiSystem : public System, Receiver<sf::Event> {

    public:
        ~RenderImguiSystem() override;

        void start() override;
        void draw() override;

    private:
        void receive(const sf::Event& event) override;
    };
}

#endif //ENGINE_RENDERIMGUISYSTEM_H
