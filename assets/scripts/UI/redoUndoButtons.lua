require('assets/scripts/object')
require('assets/scripts/vector')
require('assets/scripts/Utility/uiUtilities')

RedoUndoButtons = Object:new()

function RedoUndoButtons:init()	
	self:createButtons()
end

function RedoUndoButtons:hideDuringResultScreen()
	if self.redoUndoPanel then
		self.redoUndoPanel:get("UIRect").isEnabled = false
	end
end

function RedoUndoButtons:createButtons()

	local level = self.level

    local buttonSize = {x = 80, y = 80}
    local buttonHalfSize = {x = buttonSize.x * 0.5, y = buttonSize.y * 0.5}

	local panelPosition = {180, 100}

	self.redoUndoPanel = Game.makeActor {
		Name = "RedoUndoPanel",
		Transform = {},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {0, 0},
            offsetMin = panelPosition,
            offsetMax = panelPosition
		}
	}

	local function makeButton(name, texturePath, offset, update)

		local child = Game.makeActor {
			Name = name.."_Image",
			Transform = {},
			UIRect = {},
			Sprite = {
				material = {
					shader	= "sprite",
					texture	= texturePath,
				}
			}
		}

		local actor = Game.makeActor {
			Name = name,
			Transform = { parent = self.redoUndoPanel },
			UIRect = {
				anchorMin = {0, 0},
				anchorMax = {0, 0},
				offsetMin = {-buttonHalfSize.x + offset, -buttonHalfSize.y},
				offsetMax = { buttonHalfSize.x + offset,  buttonHalfSize.y}
			},
			LuaBehavior = {
				start = function(self)

					self.transform = self.actor:get("Transform")
					self.uiRect    = self.actor:get("UIRect")

					self.imageActor     = child
					self.imageTransform = child:get("Transform")
					self.imageUIRect    = child:get("UIRect")
				end,
				update = update,
				onMouseEnter = function(self)

					if not level.player.canControl then return end

					self.imageActor:tweenComplete()
					self.imageTransform:tweenScale({1.2, 1.2, 1.2}, 0.05)
				end,
				onMouseLeave = function(self)

					if not level.player.canControl then return end

					self.imageActor:tweenComplete()
					self.imageTransform:tweenScale({1,1,1}, 0.05)
				end
			}
		}

		child:get("Transform").parent = actor
		return actor
	end

	self.undoButton = makeButton("UndoButton", "textures/Undo.png", -buttonHalfSize.x, function(self, dt)

		if not level.player.canControl then
			return
		end

		if Game.keyboard.isDown('q') or (Game.mouse.isDown(1) and self.uiRect.isMouseOver) then

			self.imageActor:tweenComplete()
			self.imageTransform:tweenScale(Vector.mul(self.imageTransform.scale, 1.2), 0.2, Ease.punch)
			Config.audio.ui.buttonPress:play()
			level.player:undoMove()
		end
	end)

	self.redoButton = makeButton("RedoButton", "textures/Redo.png", buttonHalfSize.x, function(self, dt)

		if not level.player.canControl then
			return
		end

		if Game.keyboard.isDown('e') or (Game.mouse.isDown(1) and self.uiRect.isMouseOver) then

			self.imageActor:tweenComplete()
			self.imageTransform:tweenScale(Vector.mul(self.imageTransform.scale, 1.2), 0.2, Ease.punch)
			Config.audio.ui.buttonPress:play()
			level.player:redoMove()
		end
	end)
end