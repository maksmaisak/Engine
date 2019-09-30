//
// Created by Maksym Maisak on 30/9/19.
//

#ifndef ENGINE_EXPOSETOLUA_H
#define ENGINE_EXPOSETOLUA_H

namespace en {

    /// Exposes functionality to the LuaState of the given Engine
    class ExposeToLua {

    public:
        static void expose(class Engine& engine);
    };
}

#endif //ENGINE_EXPOSETOLUA_H
