//
// Created by Maksym Maisak on 30/12/18.
//

#ifndef ENGINE_LUASCENE_H
#define ENGINE_LUASCENE_H

#include "Scene.h"
#include <string>
#include "LuaState.h"
#include "Actor.h"
#include "Receiver.h"
#include "Collision.h"
#include "LuaReference.h"

namespace en {

    class LuaScene : public Scene, Receiver<Collision> {

    public:
        LuaScene(LuaState& lua, const std::string& filename);
        LuaScene(LuaReference&& table);
        virtual ~LuaScene() = default;

        void open() override;
        void update(float dt) override;

    private:

        void receive(const Collision& info) override;

        bool pushTableOnStack();

        LuaReference m_self;
        LuaReference m_update;
        LuaReference m_onCollision;
    };
}

#endif //ENGINE_LUASCENE_H
