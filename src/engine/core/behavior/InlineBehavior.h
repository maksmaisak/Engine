//
// Created by Maksym Maisak on 29/9/19.
//

#ifndef ENGINE_INLINEBEHAVIOR_H
#define ENGINE_INLINEBEHAVIOR_H

#include "Behavior.h"
#include <functional>

namespace en {

    class InlineBehavior : public Behavior {

    public:
        using update_t = std::function<void(Actor&, float)>;
        using draw_t = std::function<void(Actor&)>;

        inline static constexpr struct {} Update;
        inline static constexpr struct {} Draw;

        InlineBehavior(Actor& actor, decltype(Update), update_t update);
        InlineBehavior(Actor& actor, decltype(Draw), draw_t draw);
        InlineBehavior(Actor& actor, const update_t& update);

        void update(float dt) override;
        void draw() override;

    private:
        update_t m_update;
        draw_t m_draw;
    };
}

#endif //ENGINE_INLINEBEHAVIOR_H
