--
-- User: maks
-- Date: 2019-09-21
-- Time: 22:53
--

require('math')

local scene = {}

function scene:start()

    local inputs = {
        right = {x =  1, y =  0},
        left  = {x = -1, y =  0},
        up    = {x =  0, y =  1},
        down  = {x =  0, y = -1}
    }

    Game.makeActor {
        Name = "camera",
        Transform = {
            position = { x = 0, y = 0, z = 10 }
        },
        Camera = {
            isOrthographic = true
        },
        LuaBehavior = {
            update = function(self, dt)

                for key, delta in pairs(inputs) do
                    if Game.keyboard.isHeld(key) then

                        local multiplier = dt * 4;
                        self.actor:get("Transform"):move(delta.x * multiplier, delta.y * multiplier, 0)
                    end
                end

                self.actor:get("Camera").orthographicHalfSize = 10 + 5 * math.sin(Game.getTime())
            end
        }
    }
end

return scene
