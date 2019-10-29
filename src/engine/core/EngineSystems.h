//
// Created by Maksym Maisak on 30/9/19.
//

#ifndef ENGINE_ENGINESYSTEMS_H
#define ENGINE_ENGINESYSTEMS_H

#include "CompoundSystem.h"
#include "CustomTypeIndex.h"
#include "Behavior.h"
#include "BehaviorSystem.h"

namespace en {

    /// A marker system, insert it in the sequence of systems to control when behaviors are executed.
    class BehaviorsSystem : public CompoundSystem {};

    /// Manages the collection of systems used in the game.
    class EngineSystems {

    public:
        explicit EngineSystems(Engine& engine);

        void start();
        void update(float dt);
        void draw();

        template<typename TSystem, typename... Args>
        TSystem& addSystem(Args&&... args);

        template<typename TSystem, typename... Args>
        std::unique_ptr<TSystem> makeSystem(Args&&... args);

        /// Makes sure there is a system to handle behaviors of a given type.
        template<typename TBehavior>
        inline bool ensureBehaviorSystem();

    private:
        class Engine* m_engine;

        CompoundSystem m_systems;
        class CompoundSystem* m_behaviors;

        utils::CustomTypeMap<struct Dummy, bool> m_behaviorSystemPresence;
    };

    template<typename TSystem, typename... Args>
    inline TSystem& EngineSystems::addSystem(Args&&... args) {

        return m_systems.addSystem<TSystem>(std::forward<Args>(args)...);
    }

    template<>
    inline BehaviorsSystem& EngineSystems::addSystem<BehaviorsSystem>() {

        auto& system = m_systems.addSystem<BehaviorsSystem>();
        m_behaviors = &system;
        return system;
    }

    template<typename TSystem, typename... Args>
    std::unique_ptr<TSystem> EngineSystems::makeSystem(Args&&... args) {

        auto system = std::make_unique<TSystem>(std::forward<Args>(args)...);
        system->init(*m_engine);
        return system;
    }

    template<typename TBehavior>
    bool EngineSystems::ensureBehaviorSystem() {

        static_assert(std::is_base_of_v<Behavior, TBehavior>);

        if (m_behaviorSystemPresence.get<TBehavior>()) {
            return false;
        }

        if (!m_behaviors) {
            addSystem<BehaviorsSystem>();
        }

        m_behaviors->addSystem<BehaviorSystem<TBehavior>>();
        m_behaviorSystemPresence.set<TBehavior>(true);
        return true;
    }
}

#endif //ENGINE_ENGINESYSTEMS_H
