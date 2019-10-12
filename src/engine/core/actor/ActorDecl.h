//
// Created by Maksym Maisak on 24/10/18.
//

#ifndef ENGINE_ACTORDECL_H
#define ENGINE_ACTORDECL_H

#include "Entity.h"
#include "EntityRegistry.h"

namespace en {

    /// A thin wrapper around an entity. Facilitates GameObject-like programming.
    class Actor final {

    public:
        static void initializeMetatable(class LuaState& lua);

        Actor() = default;
        Actor(class Engine& engine, Entity entity);

        // Defined in Actor.h
        template<typename TComponent, typename... Args>
        TComponent& add(Args&&... args);

        // Defined in Actor.h
        template<typename TComponent, typename... Args>
        TComponent& getOrAdd(Args&&... args);

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

        inline bool isValid() const {return m_engine && m_entity && m_registry && m_registry->isAlive(m_entity);}
        inline operator bool() const {return isValid();}

        inline Entity getEntity() const {return m_entity;}
        inline operator Entity() const {return m_entity;}

        void destroy();
        void destroyImmediate();

        inline Engine& getEngine() const {return *m_engine;}
        std::string getName() const;

    private:
        Engine* m_engine = nullptr;
        EntityRegistry* m_registry = nullptr;
        Entity m_entity = en::nullEntity;
    };
}


#endif //ENGINE_ACTORDECL_H
