//
// Created by Maksym Maisak on 29/9/19.
//

#include "InlineBehavior.h"

#include <utility>

using namespace en;

InlineBehavior::InlineBehavior(Actor& actor, update_t update) :
    Behavior(actor),
    m_update(std::move(update))
{}

void InlineBehavior::update(float dt) {

    if (m_update) {
        m_update(m_actor, dt);
    }
}
