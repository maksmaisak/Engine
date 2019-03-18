//
// Created by Maksym Maisak on 2019-02-01.
//

#include "ComponentReference.h"

namespace en {

    namespace ComponentReferenceDetail {

        int indexFunction(lua_State* L) {

            // 1 table
            // 2 key

            //std::string keyAsString = getAsString(L, 2);
            //std::cout << "ComponentReference: Getting using the __index function: " << keyAsString << std::endl;

            lua_pushvalue(L, lua_upvalueindex(1));
            lua_getfield(L, -1, "__index");
            lua_pushvalue(L, 1);
            lua_pushvalue(L, 2);
            lua_call(L, 2, 1);

            return 1;
        }

        int newindexFunction(lua_State* L) {

            // 1 table
            // 2 key
            // 3 value

            //std::string keyAsString = getAsString(L, 2);
            //std::cout << "ComponentReference: Setting using the __newindex function: " << keyAsString << std::endl;

            lua_pushvalue(L, lua_upvalueindex(1));
            lua_getfield(L, -1, "__newindex");
            lua_pushvalue(L, 1);
            lua_pushvalue(L, 2);
            lua_pushvalue(L, 3);
            lua_call(L, 3, 0);

            return 0;
        }
    }
}