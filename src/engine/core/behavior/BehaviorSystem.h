//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef SAXION_Y2Q1_CPP_BEHAVIORSYSTEM_H
#define SAXION_Y2Q1_CPP_BEHAVIORSYSTEM_H

#include <typeindex>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "System.h"
#include "Behavior.h"
#include "EntityEvents.h"
#include "Collision.h"
#include "UIEvents.h"
#include "Destroy.h"

namespace en {

    template<typename TBehavior>
    class BehaviorSystem : public System, Receiver<ComponentAdded<TBehavior>, Collision, MouseEnter, MouseOver, MouseLeave, MouseDown, MouseHold, MouseUp> {

        static_assert(std::is_base_of_v<Behavior, TBehavior>);

        inline void update(float dt) override {

            // If an entity got a component added and removed multiple times, m_notStarted will have duplicates.
            // Remove them.
            std::sort(m_notStarted.begin(), m_notStarted.end());
            m_notStarted.erase(std::unique(m_notStarted.begin(), m_notStarted.end()), m_notStarted.end());
            for (Entity e : m_notStarted) {
                if (auto* behavior = m_registry->tryGet<TBehavior>(e)) {
                    behavior->start();
                }
            }
            m_notStarted.clear();

            for (Entity e : m_registry->with<TBehavior>()) {
                auto& behavior = m_registry->get<TBehavior>(e);
                behavior.update(dt);
            }
        }

        inline void draw() override {

            for (Entity e : m_registry->with<TBehavior>()) {
                auto& behavior = m_registry->get<TBehavior>(e);
                behavior.draw();
            }
        }

        inline void receive(const ComponentAdded<TBehavior>& info) override {

            m_notStarted.emplace_back(info.entity);
        }

        inline void receive(const Collision& collision) override {

            if (TBehavior* a = getBehaviorForCallback(collision.a)) {
                a->onCollision(collision.b);
            }

            if (TBehavior* b = getBehaviorForCallback(collision.b)) {
                b->onCollision(collision.a);
            }
        }

        inline void receive(const MouseEnter& info) override {forwardEvent(info.entity, info);}
        inline void receive(const MouseOver&  info) override {forwardEvent(info.entity, info);}
        inline void receive(const MouseLeave& info) override {forwardEvent(info.entity, info);}
        inline void receive(const MouseDown&  info) override {forwardEvent(info.entity, info);}
        inline void receive(const MouseHold&  info) override {forwardEvent(info.entity, info);}
        inline void receive(const MouseUp&    info) override {forwardEvent(info.entity, info);}

        template<typename TEvent>
        inline void forwardEvent(Entity e, const TEvent& event) {

            if (TBehavior* const behavior = getBehaviorForCallback(e)) {
                behavior->on(event);
            }
        }

        inline TBehavior* getBehaviorForCallback(Entity e) {

            if (auto* behavior = m_registry->tryGet<TBehavior>(e)) {
                if (!m_registry->tryGet<Destroy>(e)) {
                    return behavior;
                }
            }

            return nullptr;
        }

        std::vector<Entity> m_notStarted;
    };
}


#endif //SAXION_Y2Q1_CPP_BEHAVIORSYSTEM_H
