//
// Created by Maksym Maisak on 29/9/19.
//

#include "InlineBehavior.h"

#include <utility>

using namespace en;

InlineBehavior::InlineBehavior(Actor& actor, decltype(Update), update_t update) :
    Behavior(actor),
    m_update(std::move(update))
{}

InlineBehavior::InlineBehavior(Actor& actor, decltype(Draw), draw_t draw) :
    Behavior(actor),
    m_draw(std::move(draw))
{}

InlineBehavior::InlineBehavior(Actor& actor, const update_t& update) : InlineBehavior(actor, Update, update) {}

void InlineBehavior::update(float dt) {

    if (m_update) {
        m_update(m_actor, dt);
    }
}

void InlineBehavior::draw() {

    if (m_draw) {
        m_draw(m_actor);
    }
}
