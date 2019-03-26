//
// Created by Maksym Maisak on 2019-03-04.
//

#ifndef ENGINE_TWEENSYSTEM_H
#define ENGINE_TWEENSYSTEM_H

#include "System.h"

namespace en {

    class TweenSystem : public System {

    public:
        void update(float dt) override;
    };
}

#endif //ENGINE_TWEENSYSTEM_H
