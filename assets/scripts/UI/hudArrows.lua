require('assets/scripts/object')

HudArrows = Object:new()

function HudArrows:init()
	self.arrows = {up = {}, left = {}, down = {}, right = {}}
	print("Arrow size"..#self.arrows)
	self:createArrows()
end

local arrowSize = {x = 80, y = 80}
local arrowHalfSize = {x = arrowSize.x * 0.5, y = arrowSize.y * 0.5}

function HudArrows:hideDuringResultScreen()
	if self.hudArrowsPanel then
		self.hudArrowsPanel:get("UIRect").isEnabled = false
	end
end

function HudArrows:dropKey(key)

	local actor = self.arrows[key].actor
	actor:tweenKill()
	actor:get("Transform"):tweenScale({0,0,0}, 0.3, Ease.inOutQuart)
end

function HudArrows:pickUpKey(key)

	local actor = self.arrows[key].actor
	actor:tweenKill()
	actor:get("Transform"):tweenScale({1,1,1}, 0.3, Ease.inOutQuart)
end

function HudArrows:keyPressed(key)

	local actor = self.arrows[key].actor
	actor:tweenComplete()
	actor:get("Transform"):tweenScale({1.2, 1.2, 1.2}, 0.2, Ease.punch)
end

function HudArrows:createArrows()

	local hudArrowsPanelPosition = {-220, 100}

    self.hudArrowsPanel = Game.makeActor {
		Name = "ArrowsPanel",
		Transform = {},
		UIRect = {
			anchorMin = {1, 0},
			anchorMax = {1, 0},
			offsetMin = hudArrowsPanelPosition,
			offsetMax = hudArrowsPanelPosition
		},
	}

	local function createArrow(position, texture)

		return Game.makeActor {
			Name = "Arrow",
			Transform = { parent = self.hudArrowsPanel },
			UIRect = {
				anchorMin = {0,0},
				anchorMax = {0,0},
				offsetMin = {position.x - arrowHalfSize.x, position.y - arrowHalfSize.y},
				offsetMax = {position.x + arrowHalfSize.x, position.y + arrowHalfSize.y},
			},
			Sprite = {
				material = {
					shader	= "sprite",
					texture	= texture
				}
			}
		}
	end

	self.arrows["up"   ].actor = createArrow({x = 0, y = arrowSize.y }, "textures/keyUp.png"   )
	self.arrows["left" ].actor = createArrow({x = -arrowSize.x, y = 0}, "textures/keyLeft.png" )
	self.arrows["down" ].actor = createArrow({x = 0, y = 0 }          , "textures/keyDown.png" )
	self.arrows["right"].actor = createArrow({x =  arrowSize.x, y = 0}, "textures/keyRight.png")
end