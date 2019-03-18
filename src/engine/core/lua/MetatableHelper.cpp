//
// Created by Maksym Maisak on 3/1/19.
//

#include "MetatableHelper.h"

namespace lua {

    namespace detail {

        void printValue(lua_State* L, int index = -1) {

            int typeId = lua_type(L, index);
            switch (typeId) {
                case LUA_TSTRING:
                    printf("%d:`%s'\n", index, lua_tostring(L, index));
                    break;
                case LUA_TBOOLEAN:
                    printf("%d: %s\n", index, lua_toboolean(L, index) ? "true" : "false");
                    break;
                case LUA_TNUMBER:
                    printf("%d: %g\n", index, lua_tonumber(L, index));
                    break;
                default:
                    printf("%d: %s\n", index, lua_typename(L, typeId));
                    break;
            }
        }

        /// The __index function: (table, key) -> value
        /// Try using a property getter from the first upvalue (a table of getters).
        /// If that doesn't work, try rawgetting it, if getting from a table.
        /// If that doesn't work, look it up in the second upvalue (a storage table).
        int indexFunction(lua_State* L) {

            luaL_checkany(L, 1); // table or userdata
            luaL_checkany(L, 2); // key
            const int gettersIndex = lua_upvalueindex(1);
            const int storageIndex = lua_upvalueindex(2);

            lua_pushvalue(L, 2);
            lua_gettable(L, gettersIndex);
            if (lua_isfunction(L, -1)) {

                //std::cout << "Getting with custom getter: " << getAsString(L, 2) << std::endl;

                // call the getter
                lua_pushvalue(L, 1); // table
                lua_pushvalue(L, 2); // key
                lua_call(L, 2, 1); // getter(table, key)

                return 1;
            }
            lua_pop(L, 1);

            if (lua_istable(L, 1)) {

                //std::cout << "Getting from table: " << getAsString(L, 2) << std::endl;

                lua_pushvalue(L, 2);
                lua_rawget(L, 1);
                return 1;
            }

            if (!lua_isnil(L, storageIndex)) {

                //std::cout << "Getting from metatable: " << getAsString(L, 2) << std::endl;

                lua_pushvalue(L, 2);
                lua_gettable(L, storageIndex); // get from metatable
                if (!lua_isnil(L, -1)) {
                    return 1;
                }
            }

            //std::cout << "No custom getter and no value for key: " << getAsString(L, 2) << std::endl;
            return 0;
        }

        /// The __index function: (udata, key, value) -> ()
        /// Try using a property setter from the first upvalue (a table of setters).
        /// If that doesn't work, then try just rawsetting it (if assigning to a table).
        int newindexFunction(lua_State* L) {

            luaL_checkany(L, 1); // table or userdata
            luaL_checkany(L, 2); // key
            luaL_checkany(L, 3); // value
            const int settersIndex = lua_upvalueindex(1);

            // get a property setter from setters
            lua_pushvalue(L, 2);
            lua_gettable(L, settersIndex);
            if (lua_isfunction(L, -1)) {

                //std::cout << "Setting with custom setter: " << getAsString(L, 2) << ", " << getAsString(L, 3) << std::endl;

                lua_pushvalue(L, 1); // table
                lua_pushvalue(L, 3); // value
                lua_call(L, 2, 0);

                return 0;
            }
            lua_pop(L, 1);

            //lua_pop(L, 2); // remove __setters, nil
            if (lua_istable(L, 1)) {

                //std::cout << "Rawsetting to table: " << getAsString(L, 2) << ", " << getAsString(L, 3) << std::endl;

                lua_pushvalue(L, 2);
                lua_pushvalue(L, 3);
                lua_rawset(L, 1);

                return 0;
            }

            return 0;
        }

        int indexFunctionForward(lua_State* L) {

            // 1 table
            // 2 key

            //std::string keyAsString = getAsString(L, 2);
            //std::cout << "indexFunctionForward: Getting using the __index function: " << keyAsString << std::endl;

            lua_pushvalue(L, lua_upvalueindex(1));
            lua_getfield(L, -1, "__index");
            lua_pushvalue(L, 1);
            lua_pushvalue(L, 2);
            lua_call(L, 2, 1);

            return 1;
        }

        int newindexFunctionForward(lua_State* L) {

            // 1 table
            // 2 key
            // 3 value

            //std::string keyAsString = getAsString(L, 2);
            //std::cout << "newindexFunctionForward: Setting using the __newindex function: " << keyAsString << std::endl;

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