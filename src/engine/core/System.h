//
// Created by Maksym Maisak on 22/10/18.
//

#ifndef SAXION_Y2Q1_CPP_SYSTEM_H
#define SAXION_Y2Q1_CPP_SYSTEM_H

#include <SFML/Graphics.hpp>

namespace en {

    class System {

    public:
        virtual ~System() = default;
        virtual void start() {} // called once before the first update
        virtual void update(float dt) {} // called every fixed timestep
        virtual void draw() {} // called every frame

    protected:
        class Engine* m_engine = {};
        class EntityRegistry* m_registry = {};

        sf::RenderWindow& getWindow();

    private:
        void init(Engine& engine);
        friend class Engine;
        friend class CompoundSystem;
    };
}

#endif //SAXION_Y2Q1_CPP_SYSTEM_H
