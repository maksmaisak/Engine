--
-- User: maks
-- Date: 30/12/18
-- Time: 20:15
--

require('assets/scripts/vector')

local planeRenderInfo = {
    mesh = "models/plane.obj",
    material = Game.makeMaterial {
        shader = "pbr",
        albedo             = "textures/testPBR/oldTiledStone/tiledstone1_basecolor.png",
        metallicSmoothness = "textures/testPBR/oldTiledStone/tiledstone1_metallicSmoothness.psd",
        normal             = "textures/testPBR/oldTiledStone/tiledstone1_normal.png",
        ao                 = "textures/testPBR/oldTiledStone/tiledstone1_AO.png",
    },
    isBatchingStatic = true
}

local scenery = {
    {
        Name = "plane",
        Transform = {
            scale = { x = 10, y = 10, z = 10 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "character",
        Transform = {
            position = { x = 0, y = 0, z = 0 },
            scale = {3, 3, 3},
            children = {
                {
                    Name = "head",
                    Transform = {
                        position = { x = 0, y = 1, z = 0 }
                    }
                }
            }
        },
        RenderInfo = {
            mesh = "objects/Player/Main_Character.obj",
            material = {
                shader = "pbr",
                albedo             = 'objects/Player/Texture/Main_Character_01_AlbedoTransparency.png',
                metallicSmoothness = 'objects/Player/Texture/Main_Character_01_MetallicSmoothness.png',
                ao                 = 'objects/Player/Texture/Main_Character_01_AO.png',
                normal             = 'objects/Player/Texture/Main_Character_01_Normal.png',
                smoothnessMultiplier = 1,
                aoMultiplier = 1,
            },
            isBatchingStatic = true
        }
    }
}

local scene = {}

function scene:start()

    Game.makeActors(scenery)

    Game.makeActor {
        Name = "camera",
        Transform = {
            position = {x = 0, y = 0, z = 5}
        },
        Camera = {},
        CameraOrbitBehavior = {
            target   = "head",
            distance =  5,
            minTilt  = -60,
            maxTilt  =  60,
        }
    }

    local rotationCenter = Game.makeActor({Transform = {position = {0, 2, 0}}})
    self.rotationCenterTransform = rotationCenter:get("Transform")

    Game.makeActor {
        Name = "Light",
        Transform = {
            parent = rotationCenter,
            position = {-3, 0, 0},
            scale = {0.1, 0.1, 0.1},
        },
        RenderInfo = {
            mesh = "models/sphere2.obj",
            material = Game.makeMaterial {
                shader = "color",
                color = {1, 0, 0}
            }
        },
        Light = {
            color = {1, 0, 0},
            intensity = 10
        }
    }

    Game.makeActor {
        Name = "Light2",
        Transform = {
            position = {3, 2, 0},
            scale = {0.1, 0.1, 0.1}
        },
        RenderInfo = {
            mesh = "models/sphere2.obj",
            material = Game.makeMaterial {
                shader = "color",
                color = {0, 1, 0}
            }
        },
        Light = {
            color = {0, 1, 0},
			intensity = 10
		}
    }

    Game.makeActor {
        Name = "LightDirectional",
        Transform = {
            rotation = {45, 0, 0}
        },
        Light = {
            kind = "directional",
            intensity = 2
        }
    }
end

function scene:update(dt)

    if Game.keyboard.isHeld("Left") then
        self.rotationCenterTransform:rotate(-45 * dt, 0, 1, 0)
    elseif Game.keyboard.isHeld("Right") then
        self.rotationCenterTransform:rotate( 45 * dt, 0, 1, 0)
    end

    if Game.keyboard.isHeld("Up") then
        Game.find("Light2"):get("Transform"):move(0, 2 * dt, 0)
    elseif Game.keyboard.isHeld("Down") then
        Game.find("Light2"):get("Transform"):move(0, -2 * dt, 0)
    end
end

scene.ambientLighting = {color = Vector.from {1.010478, 1.854524, 2.270603} * 0.5}

return scene