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
    }
}

local defaultMaterial = Game.makeMaterial {shader = "pbr"}

local scenery = {
    {
        Name = "camera",
        Transform = {
            position = { x = 0, y = 0, z = 10 }
        },
        Camera = {},
        CameraOrbitBehavior = {
            target   = "center",
            distance =  10,
            minTilt  = -60,
            maxTilt  =  60,
        }
    },
    {
        Name = "plane",
        Transform = {
            position = { x = 0 , y = 0 , z = 0 },
            scale    = { x = 10, y = 10, z = 10 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "wallXP",
        Transform = {
            position = { 10, 10, 0 },
            scale    = { 10, 10, 10 },
            rotation = { 0, 0, 90}
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "wallXN",
        Transform = {
            position = { -10, 10, 0 },
            scale    = { 10, 10, 10 },
            rotation = { 0, 0, -90}
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "wallZP",
        Transform = {
            position = { 0  , 10, 10 },
            scale    = { 10 , 10, 10 },
            rotation = { -90, 0 , 0  }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "wallZN",
        Transform = {
            position = { 0, 10, -10 },
            scale    = { 10, 10, 10 },
            rotation = { 90,  0,  0 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "ceiling",
        Transform = {
            position = { 0  , 20, 0  },
            scale    = { 10 , 10, 10 },
            rotation = { 180, 0 , 0  }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "center",
        Transform = {position = {0, 5, 0}},
    }
}

local scene = {}

function scene:start()

    Game.makeActors(scenery)

    local lightActor = Game.makeActor {
        Name = "Light",
        Transform = {
            position = {-8, 2, -8},
            scale = {0.1, 0.1, 0.1}
        },
        RenderInfo = {
            mesh = "models/sphere2.obj",
            material = defaultMaterial
        },
        Light = {
            color = {1, 0, 0},
            intensity = 40
        }
    }
    local light = lightActor:get("Light")
    light.intensity = 0
    light:tweenIntensity(40):setLoopBounce()

    local lightActor2 = Game.makeActor {
        Name = "Light2",
        Transform = {
            position = {8, 2, -8},
            scale = {0.1, 0.1, 0.1}
        },
        RenderInfo = {
            mesh = "models/sphere2.obj",
            material = defaultMaterial
        },
        Light = {
            color = {0, 1, 0},
			intensity = 40
		}
    }
    local light2 = lightActor2:get("Light")
    light2.intensity = 0
    light2:tweenIntensity(40, 10, Ease.fluctuate):setLoopBounce()

    Game.makeActor {
        Name = "LightAmbient",
        Transform = {},
        Light = {
            kind = "directional",
            intensity = 0,
            ambientColor = {0.02, 0.02, 0.02}
        }
    }

    Game.makeActor {
        Name = "LightDirectional",
        Transform = {
            rotation = {45, 0, 0}
        },
        Light = {
            kind = "directional",
            intensity = 1
        }
    }

    Game.makeActor {
        Name = "LightDirectional2",
        Transform = {
            rotation = {0, 0, 0}
        },
        Light = {
            kind = "directional",
            intensity = 0.1
        }
    }

    local tweenTestRenderInfo = {
        mesh = "models/sphere2.obj",
        material = Game.makeMaterial {
            shader = "pbr",
            albedoColor = {1, 0, 0}
        }
    }

    local function makeTweenTest(position, delta, ease, tweenDuration)

        local actor = Game.makeActor {
            Name = "TweenTest",
            Transform = {
                position = position,
                scale = {0.2, 0.2, 0.2}
            },
            RenderInfo = tweenTestRenderInfo,
        }

        actor:get("Transform"):tweenPosition(position + delta, tweenDuration or 1, ease):setLoopBounce()

        return actor
    end

    local i = 0
    local function skipLine() i = i + 1 end
    local function makeTweenTestOnWall(ease, tweenDuration)

        local actor = makeTweenTest(
            Vector.from {-5, 18 - i * 0.5, -10},
            Vector.from {10, 0, 0},
            ease,
            tweenDuration
        )

        i = i + 1

        return actor
    end

    makeTweenTestOnWall(Ease.linear)
    skipLine()
    makeTweenTestOnWall(Ease.inQuad )
    makeTweenTestOnWall(Ease.inCubic)
    makeTweenTestOnWall(Ease.inQuart)
    makeTweenTestOnWall(Ease.inQuint)
    makeTweenTestOnWall(Ease.inExpo )
    makeTweenTestOnWall(Ease.inCirc )
    makeTweenTestOnWall(Ease.inSine )
    skipLine()
    makeTweenTestOnWall(Ease.outQuad )
    makeTweenTestOnWall(Ease.outCubic)
    makeTweenTestOnWall(Ease.outQuart)
    makeTweenTestOnWall(Ease.outQuint)
    makeTweenTestOnWall(Ease.outExpo )
    makeTweenTestOnWall(Ease.outCirc )
    makeTweenTestOnWall(Ease.outSine )
    skipLine()
    makeTweenTestOnWall(Ease.inOutQuad )
    makeTweenTestOnWall(Ease.inOutCubic)
    makeTweenTestOnWall(Ease.inOutQuart)
    makeTweenTestOnWall(Ease.inOutQuint)
    makeTweenTestOnWall(Ease.inOutExpo )
    makeTweenTestOnWall(Ease.inOutCirc )
    makeTweenTestOnWall(Ease.inOutSine )
    skipLine()
    makeTweenTestOnWall(Ease.punch)
    makeTweenTestOnWall(Ease.fluctuate, 1)
end

return scene