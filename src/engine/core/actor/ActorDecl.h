//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef SAXION_Y2Q1_CPP_ACTORDECL_H
#define SAXION_Y2Q1_CPP_ACTORDECL_H

#include <type_traits>
#include "Entity.h"
#include "EntityRegistry.h"

namespace en {

    class Behavior;
    class Engine;
    class LuaState;

    template<typename T>
    constexpr inline bool isBehavior = std::is_base_of<Behavior, T>::value;

    /// A thin wrapper around an entity. Facilitates GameObject-like programming.
    class Actor final {

    public:
        static void initializeMetatable(LuaState& lua);

        Actor() = default;
        Actor(Engine& engine, Entity entity);

        // Defined in Actor.h
        template<typename TComponent, typename... Args>
        TComponent& add(Args&&... args);

        template<typename TComponent, typename... Args>
        inline TComponent& getOrAdd(Args&&... args) {return m_registry->getOrAdd<TComponent>(m_entity);}

        template<typename TComponent>
        inline TComponent& get() {return m_registry->get<TComponent>(m_entity);}

        template<typename TComponent>
        inline const TComponent& get() const {return m_registry->get<TComponent>(m_entity);}

        template<typename TComponent>
        inline TComponent* tryGet() {return m_registry->tryGet<TComponent>(m_entity);}

        template<typename TComponent>
        inline const TComponent* tryGet() const {return m_registry->tryGet<TComponent>(m_entity);}

        template<typename TComponent>
        inline bool remove() {return m_registry->remove<TComponent>(m_entity);}

        inline bool isValid() const {return m_engine && !isNullEntity(m_entity) && m_registry->isAlive(m_entity);}

        void destroy();
        void destroyImmediate();

        inline Engine& getEngine() const {return *m_engine;}
        std::string getName() const;

        inline operator Entity() const {return m_entity;}
        inline operator bool() const {return isValid();}

    private:
        Engine* m_engine = nullptr;
        EntityRegistry* m_registry = nullptr;
        Entity m_entity = en::nullEntity;
    };
}


#endif //SAXION_Y2Q1_CPP_ACTORDECL_H
