--
-- User: maks
-- Date: 2019-02-14
-- Time: 13:16
--

local planeRenderInfo = {
    mesh = "models/plane.obj",
    material = Game.makeMaterial {
        diffuse = "textures/land.jpg",
        specularColor = {0.05, 0.08, 0.05},
        shininess = 10
    }
}

local scenery = {
    {
        Name = "camera",
        Transform = {
            position = { x = 0, y = 0, z = 10 }
        },
        Camera = {},
        CameraOrbitBehavior = {
            target   = "head",
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
            position = { 0, 10, 10 },
            scale    = { 10, 10, 10 },
            rotation = { -90, 0, 0 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "wallZN",
        Transform = {
            position = { 0, 10, -10 },
            scale    = { 10, 10, 10 },
            rotation = { 90, 0, 0 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "ceiling",
        Transform = {
            position = { 0, 20, 0 },
            scale    = { 10, 10, 10 },
            rotation = { 180, 0, 0 }
        },
        RenderInfo = planeRenderInfo
    },
    {
        Name = "cube",
        Transform = {
            position = { x = 0, y = 2, z = 0 },
            scale    = { x = 1, y = 1, z = 1 }
        },
        RenderInfo = {
            mesh = "models/cube_flat.obj",
            material = {
                diffuse = "textures/container/diffuse.png",
                specular = "textures/container/specular.png",
                shininess = 100
            }
        },
    },
    {
        Name = "head",
        Transform = {
            position = { x = 0, y = 5, z = 0 }
        },
        RenderInfo = {
            mesh = "models/suzanna_flat.obj",
            material = {
                diffuse = "textures/bricks.jpg",
                shininess = 100
            }
        }
    },
    {
        Name = "teapot",
        Transform = {
            position = { x = 0, y = 7, z = 0 }
        },
        RenderInfo = {
            mesh = "models/teapot_smooth.obj",
            material = {
                diffuse = "textures/bricks.jpg"
            }
        }
    }
}

local scene = {}

function scene:start()

    Game.makeActors(scenery)

    Game.makeActor {
        Name = "Light",
        Transform = {
            scale = {0.1, 0.1, 0.1}
        },
        RenderInfo = {
            mesh = "models/sphere2.obj",
            material = {}
        },
        Light = {
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
            material = {}
        },
        Light = {
            intensity = 10
        }
    }

    Game.makeActor {
        Name = "LightDirectional",
        Transform = {
            scale = {0.1, 0.1, 0.1},
            rotation = {-45, 0, 0}
        },
        Light = {
            kind = "directional",
            ambientColor = {0, 0, 0},
            intensity = 0.1
        }
    }

    Game.makeActor {
        Name = "LightDirectional2",
        Transform = {
            scale = {0.1, 0.1, 0.1},
            rotation = {0, 0, 0}
        },
        Light = {
            kind = "directional",
            ambientColor = {0, 0, 0},
            intensity = 0.1
        }
    }

    Game.makeActor {
        Name = "Panel",
        Transform = {
            children = {
                {
                    Name = "FontAtlasView",
                    Transform = {},
                    UIRect = {
                        anchorMin = {0, 0},
                        anchorMax = {1, 1},
                        offsetMin = {0, 0},
                        offsetMax = {0, -60}
                    },
                    Sprite = {
                        material = {
                            shader = "sprite",
                            texture = "textures/transparent.png"
                        }
                    },
                    LuaBehavior = {
                        onMouseEnter = function(self)
                            self.actor:get("Sprite").isEnabled = false
                        end,
                        onMouseLeave = function(self)
                            self.actor:get("Sprite").isEnabled = true
                        end
                    }
                },
                {
                    Name = "FontAtlasViewTitle",
                    Transform = {},
                    UIRect = {
                        anchorMin = {0, 1},
                        anchorMax = {1, 1},
                        offsetMin = {0, -60},
                        offsetMax = {0, 0}
                    },
                    Sprite = {
                        material = {
                            shader = "sprite",
                            texture = "textures/white.png"
                        }
                    },
                    Text = {
                        string = "Font atlas:",
                        font = "fonts/Menlo.ttc",
                        fontSize = 50,
                        color = {0,0,0,1}
                    }
                }
            }
        },
        UIRect = {
            anchorMin = {0, 0},
            anchorMax = {1, 1},
            offsetMin = { 100,  200},
            offsetMax = {-100, -100}
        },
        Sprite = {
            material = {
                shader = "sprite",
                texture = "textures/black.png"
            }
        }
    }

    Game.makeActor {
        Name = "PanelBottom",
        Transform = {},
        UIRect = {
            anchorMin = {0, 0},
            anchorMax = {1, 0},
            offsetMin = {50 , 0},
            offsetMax = {-50, 100}
        },
        Sprite = {
            material = {
                shader = "sprite",
                texture = "textures/black.png"
            }
        },
        LuaBehavior = {
            update = function(self)
                self.actor:get("UIRect").isEnabled = not self.actor:get("UIRect").isMouseOver
            end
        }
    }

    Game.makeActor {
        Name = "TextElement",
        Transform = {parent = "PanelBottom"},
        UIRect = {},
        Text = {
            font = "fonts/Menlo.ttc",
            color = {0, 1 / 3, 2 / 3, 1},
            string = "The quick brown fox jumps over the lazy dog\nThe quick brown fox jumps over the lazy dog"
        },
        LuaBehavior = {
            update = function(self, dt)

                local text = self.actor:get("Text")
                text.string = string.sub("The quick brown fox jumps over the lazy dog\nThe quick brown fox jumps over the lazy dog", 1, 1 + math.floor(Game.getTime() * 10))
                --text.font = math.fmod(Game.getTime(), 2) < 1 and "fonts/Menlo.ttc" or "fonts/arial.ttf"

                local color = text.color
                color.x = math.fmod(color.x + dt * 0.1, 1)
                color.y = math.fmod(color.y + dt * 0.1, 1)
                color.z = math.fmod(color.z + dt * 0.1, 1)
                text.color = color

                text.fontSize = math.floor(30 + math.sin(Game.getTime()) * 20)
            end
        }
    }
end

local function animateUITransform(actor)

    local tf = actor:get("Transform")
    tf.rotation = {0, 0, 10 * math.sin(Game.getTime() * 8) }
    local scale = 1 + 0.03 * math.sin(Game.getTime() * 8)
    tf.scale = {scale, scale, scale }

    local uiRect = actor:get("UIRect")
    uiRect.pivot = {
        math.sin(Game.getTime() * 0.1) * 0.5 + 0.5,
        math.cos(Game.getTime() * 0.1) * 0.5 + 0.5
    }
end

function scene:update(dt)

    Game.find("Light"):get("Transform").position = {-4, 3 + 2 * math.sin(Game.getTime()), 0}
    --Game.find("LightDirectional"):get("Transform"):rotate(45 * dt, 1, 0, 0);

    if Game.keyboard.isHeld("Up") then
        Game.find("Light2"):get("Transform"):move(0, 2 * dt, 0)
    elseif Game.keyboard.isHeld("Down") then
        Game.find("Light2"):get("Transform"):move(0, -2 * dt, 0)
    end

    --Game.find("Panel"):get("UIRect").isEnabled = math.fmod(Game.getTime(), 2) < 1
    animateUITransform(Game.find("Panel"))
end

return scene