//
// Created by Maksym Maisak on 6/10/19.
//

#ifndef ENGINE_GLOBALCOMPONENTS_H
#define ENGINE_GLOBALCOMPONENTS_H

#include "Engine.h"

namespace en {

    /// A helper for getting components which are meant to be used as singletons or global services.
    class GlobalComponents {

        struct GlobalsMarker {};

    public:
        template<class TComponent, typename... Args>
        inline static TComponent& getOrAddGlobalComponent(Engine& engine, Args&&... args) {

            Actor globals = engine.actor(engine.getRegistry().with<GlobalsMarker>().tryGetOne());
            if (!globals) {
                globals = engine.makeActor("GlobalComponents");
                globals.add<GlobalsMarker>();
            }

            return globals.getOrAdd<TComponent>(std::forward<Args>(args)...);
        }
    };
}

#endif //ENGINE_GLOBALCOMPONENTS_H
