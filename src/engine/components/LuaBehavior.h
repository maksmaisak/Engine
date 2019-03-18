//
// Created by Maksym Maisak on 2019-01-31.
//

#ifndef SAXION_Y2Q2_RENDERING_LUABEHAVIOR_H
#define SAXION_Y2Q2_RENDERING_LUABEHAVIOR_H

#include "ComponentsToLua.h"
#include "LuaState.h"
#include "LuaReference.h"

namespace en {

    class LuaBehavior : public Behavior {

        LUA_TYPE(LuaBehavior)

    public:

        static LuaBehavior& addFromLua(Actor& actor, LuaState& lua);
        static void initializeMetatable(LuaState& lua);

        LuaBehavior(en::Actor actor);
        LuaBehavior(en::Actor actor, LuaReference&& table);

        void start() override;
        void update(float dt) override;
        void onCollision(Entity other) override;

        void on(const MouseEnter& enter) override;
        void on(const MouseOver&  over ) override;
        void on(const MouseLeave& leave) override;
        void on(const MouseDown&  down ) override;
        void on(const MouseHold&  hold ) override;
        void on(const MouseUp&    up   ) override;

    private:

        static int indexFunction(lua_State*);
        static int newindexFunction(lua_State*);

        LuaReference m_self;

        LuaReference m_start;
        LuaReference m_update;
        LuaReference m_onCollision;

        LuaReference m_onMouseEnter;
        LuaReference m_onMouseOver;
        LuaReference m_onMouseLeave;
        LuaReference m_onMouseDown;
        LuaReference m_onMouseHold;
        LuaReference m_onMouseUp;
    };
}

#endif //SAXION_Y2Q2_RENDERING_LUABEHAVIOR_H
