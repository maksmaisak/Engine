//
// Created by Maksym Maisak on 27/10/19.
//

#ifndef ENGINE_RENDERIMGUISYSTEM_H
#define ENGINE_RENDERIMGUISYSTEM_H

#include "System.h"
#include "Receiver.h"

namespace en {

    class RenderImguiSystem : public System {

    public:
        ~RenderImguiSystem() override;

        void start() override;
        void draw() override;
    };
}

#endif //ENGINE_RENDERIMGUISYSTEM_H
