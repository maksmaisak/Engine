//
// Created by Maksym Maisak on 22/10/18.
//

#include "Behavior.h"
#include "Actor.h"
#include "Engine.h"

namespace en {

    Behavior::Behavior(const Actor& actor) :
        m_actor(actor),
        m_engine(&m_actor.getEngine()),
        m_registry(&m_engine->getRegistry()) {}
}