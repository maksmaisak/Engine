//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_INLINEBEHAVIOR_H
#define ENGINE_INLINEBEHAVIOR_H

#include "Behavior.h"
#include <functional>

namespace en {

    class InlineBehavior : public Behavior {

        using update_t = std::function<void(Actor&, float)>;

    public:
        InlineBehavior(Actor& actor, update_t update);
        void update(float dt) override;

    private:
        update_t m_update;
    };
}

#endif //ENGINE_INLINEBEHAVIOR_H
