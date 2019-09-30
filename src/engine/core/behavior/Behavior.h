//
// Created by Maksym Maisak on 17/10/18.
//

#ifndef SAXION_Y2Q1_CPP_COMPONENT_H
#define SAXION_Y2Q1_CPP_COMPONENT_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <cassert>
#include "Entity.h"
#include "EntityRegistry.h"
#include "ActorDecl.h"
#include "UIEvents.h"

namespace en {

    /// Components inheriting from this will have the virtual callback functions called.
    /// WARNING. The callback functions are only guaranteed to be called if the component was added using Actor::add<TBehavior>()
    class Behavior {

    public:
        explicit Behavior(const Actor& actor);
        virtual ~Behavior() = default;

        inline Actor& getActor() { return m_actor; }
        inline Engine& getEngine() { return assert(m_engine), *m_engine; }
        inline EntityRegistry& getRegistry() { return assert(m_registry), *m_registry; }

        virtual void start() {};
        virtual void update(float dt) {};
        virtual void draw() {};
        virtual void onCollision(Entity other) {};

        virtual void on(const MouseEnter&) {};
        virtual void on(const MouseOver&) {};
        virtual void on(const MouseLeave&) {};
        virtual void on(const MouseDown&) {};
        virtual void on(const MouseHold&) {};
        virtual void on(const MouseUp&) {};

    protected:
        Actor m_actor;
        Engine* m_engine;
        EntityRegistry* m_registry;
    };
}

#endif //SAXION_Y2Q1_CPP_COMPONENT_H
