//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef ENGINE_SYSTEM_H
#define ENGINE_SYSTEM_H

namespace en {

    class System {

    public:
        virtual ~System() = default;
        virtual void start() {} // called once before the first update
        virtual void update(float dt) {} // called every fixed timestep
        virtual void draw() {} // called every frame

    protected:
        class Engine* m_engine = nullptr;
        class EntityRegistry* m_registry = nullptr;

    private:
        void init(Engine& engine);

        friend class CompoundSystem;
        friend class EngineSystems;
    };
}

#endif //ENGINE_SYSTEM_H
