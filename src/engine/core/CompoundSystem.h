//
// Created by Maksym Maisak on 2019-03-08.
//

#ifndef ENGINE_COMPOUNDSYSTEM_H
#define ENGINE_COMPOUNDSYSTEM_H

#include <vector>
#include "System.h"

namespace en {

    class CompoundSystem : public System {

    public:
        void start() override;
        void update(float dt) override;
        void draw() override;

        template<typename TSystem, typename... Args>
        TSystem& addSystem(Args&&... args);

    private:
        void startNotStartedSystems();

        std::vector<std::unique_ptr<System>> m_systems;
        std::vector<System*> m_notStarted;
    };

    template<typename TSystem, typename... Args>
    inline TSystem& CompoundSystem::addSystem(Args&&... args) {

        auto ptr = std::make_unique<TSystem>(std::forward<Args>(args)...);
        TSystem& system = *ptr;
        m_systems.push_back(std::move(ptr));

        system.init(*m_engine);
        system.start();
        return system;
    }
}


#endif //ENGINE_COMPOUNDSYSTEM_H
