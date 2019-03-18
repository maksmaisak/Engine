require('math')
require('assets/scripts/vector')
require('assets/scripts/object')
require('assets/scripts/level/map')
require('assets/scripts/UI/pauseMenu')
require('assets/scripts/UI/resultScreen')

Player = Object:new {
	gridPosition = { x = 1, y = 1 },
	lastPosition = { x = 1, y = 1 },
	map			 = nil
}

local disabledKeys = {left = false, right = false, up = false, down = false}
local inputKeys = { left = { x = 1, y = 0}, right = {x = -1, y = 0}, up = {x = 0, y = 1}, down = {x = 0, y = -1} }
local keyMaterial = Game.makeMaterial {
	shader = 'pbr',
	albedo = 'objects/key/KeyTile_AlbedoTransparency.png',
	metallicSmoothness = 'objects/key/KeyTile_MetallicSmoothness.png',
	normal = 'objects/key/KeyTile_Normal.png',
	ao     = 'objects/key/KeyTile_AO.png'
}

function Player:getPositionFromGridPosition(gridPosition)

	return Vector:new {
		x = gridPosition.x - 1,
		y = self.map:calculateYOffset(gridPosition),
		z = gridPosition.y - 1
	}
end

function Player:makeKey(gridPosition, keyName)

    local position = {
        x = gridPosition.x - 1,
        y = self.map:calculateYOffset(gridPosition),
        z = gridPosition.y - 1
    }

	local rotation = {0, 0, 0}

	if keyName == "up" then
		rotation.y = 90
	elseif keyName == "left" then
		rotation.y = 180
	elseif keyName == "down" then
		rotation.y = 270
	end

    return Game.makeActor {
        Name = "Key."..gridPosition.x.."."..gridPosition.y,
        Transform = {
            position = position,
			rotation = rotation,
        },
        RenderInfo = {
            mesh = "objects/key/Key.obj",
            material = keyMaterial
        }
	}
end

function Player:activateGoal(gridPosition)

	local gridItem	= self.map:getGridAt(gridPosition)
	local goal		= gridItem.goal
	if not goal.startActive and not gridItem.isButtonTargetEnabled then
		return
	end

	if goal.isActivated then
		return
	end

	goal.isActivated = true

	self.canControl = false
	self.level.isLevelComplete = true
	
	local resultScreen = Game.makeActor {
		Name = "ResultScreen",
		LuaBehavior = Config.resultScreen
	}

	Config.audio.levelFinished:play()

	self.level.redoUndoButtons:hideDuringResultScreen()
	self.level.hudArrows:hideDuringResultScreen()
	self.level.tutorialScreen:hideDuringResultScreen()

	local resultScreenBehavior = resultScreen:get("LuaBehavior");
	resultScreenBehavior.level = self.level
	resultScreenBehavior:activate()
end

function Player:activateButton(gridPosition)

	local button = self.map:getGridAt(gridPosition).button
	if button.isActivated then
		return
	end

	button.isActivated = true

	self:activateButtonTarget(button)

	print("Button activated")
end

function Player:disableButton(gridPosition)

	local button = self.map:getGridAt(gridPosition).button
	if not button.isActivated then
		return
	end

	-- if there is one key left on the droppable button don't do the animation
	for k, v in pairs(self.map:getDroppedKeysGridAt(gridPosition).hasKeyDropped) do
		if v then
			return
		end
	end

	button.isActivated	= false

	self:deactivateButtonTarget(button)
	print("Button Deactivated")
end

function Player:activateButtonTarget(button)

	for i, targetPosition in ipairs(button.targetPositions) do

		local target = self.map:getGridAt(targetPosition)

		target.isButtonTargetEnabled = true

		if target.goal then

			print("Activating goal")
			local goal = target.goal
			if not goal.startActive and goal.light then
				goal:activateFire()
			end

		elseif target.door then

			print("Activating door")
			local door = target.door

			Config.audio.door.open:play()
			Config.audio.door.close:stop()

			door.swingLeft :tweenKill()
			door.swingRight:tweenKill()

			door.swingLeftTransform :tweenRotation {0,  90, 0}
			door.swingRightTransform:tweenRotation {0, -90, 0}
		end
	end
end

function Player:deactivateButtonTarget(button)

	for i, targetPosition in ipairs(button.targetPositions) do

		local target = self.map:getGridAt(targetPosition)

		target.isButtonTargetEnabled = false

		if target.goal then

			print("Deactivating goal")
			local goal = target.goal
			if not goal.startActive and goal.light then
				goal:deactivateFire()
			end

		elseif target.door then

			print("Deactivating door")

			local door = target.door

			Config.audio.door.close:play()
			Config.audio.door.open:stop()

			door.swingLeft :tweenKill()
			door.swingRight:tweenKill()

			door.swingLeftTransform :tweenRotation {0, 0, 0}
			door.swingRightTransform:tweenRotation {0, 0, 0}
		end
	end
end

function Player:registerMove(undoFunction, redoFunction)

	print("move registered")

	for k, v in pairs(self.moves) do
		if k >= self.currentMoveIndex then
			self.moves[k] = nil
			v = nil
			k = nil
		end
	end

	self.currentMoveIndex = self.currentMoveIndex + 1

	self.moves[#self.moves + 1]	= { 
		undo = undoFunction,
		redo = redoFunction
	}
end

function Player:blockKey(key, canRegisterMove)

	print(key.." key blocked")
	disabledKeys[key] = true
	self.map:getDroppedKeysGridAt(self.gridPosition).hasKeyDropped[key] = true

	local pair = {}
	pair[key] = self:makeKey(self.gridPosition, key)

	local keys = self.map:getDroppedKeysGridAt(self.gridPosition).keys
	keys[#keys + 1] = pair

	self.level.hudArrows:dropKey(key)

	self.transform.position = self:getPositionFromGridPosition(self.gridPosition)

	if canRegisterMove then
		self:registerMove(
			function() self:unblockKey(key, false) end,
			function() self:blockKey  (key, false) end
		)
	end
end

function Player:unblockKey(key, canRegisterMove)

	print(key.." key unblocked")
	disabledKeys[key] = false
	self.map:getDroppedKeysGridAt(self.gridPosition).hasKeyDropped[key] = false

	self.level.hudArrows:pickUpKey(key)

	for k, v in pairs(self.map:getDroppedKeysGridAt(self.gridPosition).keys) do
		local currentActor = v[key]
		if currentActor then
			currentActor:destroy()
			currentActor = nil
			v[key] = nil
			break
		end
	end

	local index = 0

	for k, v in pairs(self.map:getDroppedKeysGridAt(self.gridPosition).keys) do
		for m, p in pairs(v) do
			if p then

				p:get("Transform").position = {
					x = self.gridPosition.x - 1,
					y = index * p:get("Transform").scale.y * 0.2,
					z = self.gridPosition.y - 1
				}

				index = index + 1
			end
		end
	end

	self.transform.position = self:getPositionFromGridPosition(self.gridPosition)

	if canRegisterMove then
		self:registerMove(
			function() self:blockKey  (key, false) end,
			function() self:unblockKey(key, false) end
		)
	end
end

function Player:moveByKey(keyFound)

	local value = inputKeys[keyFound]
	self.level.hudArrows:keyPressed(keyFound)
	self:moveByInput({x = value.x, y = value.y})
end

function Player:moveByInput(input)

	if input.x == 0 and input.y == 0 then
		return
	end

	local nextPosition = {
		x = self.gridPosition.x + input.x,
		y = self.gridPosition.y + input.y
	}

	if nextPosition.x > self.map:getGridSize().x then
		nextPosition.x = self.map:getGridSize().x
	elseif nextPosition.x < 1 then
		nextPosition.x = 1
	end

	if nextPosition.y > self.map:getGridSize().y then
		nextPosition.y = self.map:getGridSize().y
	elseif nextPosition.y < 1 then
		nextPosition.y = 1
	end

	self:moveToPosition(nextPosition, true)
end

function Player:moveToPosition(nextPosition, canRegisterMove, didUsePortal, playBackTween, rotation)

	local gridItem = self.map:getGridAt(nextPosition)

	if not gridItem or gridItem.obstacle or not gridItem.tile then
		return
	end

	if nextPosition.x == self.gridPosition.x and nextPosition.y == self.gridPosition.y then
		return
	end

	local oldLastPosition = { x = self.lastPosition.x, y = self.lastPosition.y }

	self.lastPosition.x = self.gridPosition.x
	self.lastPosition.y = self.gridPosition.y

	-- if the tile changed then we gotta check if there was a button and disable it
	if self.lastPosition.x ~= nextPosition.x or self.lastPosition.y ~= nextPosition.y then
		if self.map:getGridAt(self.lastPosition).button then
			self:disableButton(self.lastPosition)
		end
	end

	if gridItem.door and not gridItem.isButtonTargetEnabled then
		self:activateButton(self.lastPosition)

		self.lastPosition.x = oldLastPosition.x
		self.lastPosition.y = oldLastPosition.y
		return
	end

	self.gridPosition.x = nextPosition.x
	self.gridPosition.y = nextPosition.y

	local lastPosition = { x = self.lastPosition.x, y = self.lastPosition.y }
	local gridPosition = { x = self.gridPosition.x, y = self.gridPosition.y }
	local currentRotation = self.transform.rotation
	if canRegisterMove then
		self:registerMove(
			function()
				self:moveToPosition(lastPosition, false, true, true, currentRotation)
			end,
			function()
				self:moveToPosition(gridPosition, false)
			end
		)
	end

	if gridItem.button then
		self:activateButton(self.gridPosition)
	end

	if gridItem.goal then
		self:activateGoal(self.gridPosition)
	end

	if not didUsePortal and gridItem.portal then
        Config.audio.portal:play()
		self:moveToPosition(gridItem.portal.teleportPosition, false, true)
		return
	end

	self.actor:tweenComplete()

	if not playBackTween then
		self.transform:tweenJump(self:getPositionFromGridPosition(nextPosition), 1, 0.2)

		local directionX	= self.gridPosition.x - self.lastPosition.x
		local directionY	= self.gridPosition.y - self.lastPosition.y
		local angle			= math.atan(directionX, directionY) * 180 / math.pi
		local rotationTween = self.transform:tweenRotation({0, angle, 0}, 0.2, Ease.outQuad)
	else
		self.transform.position		= self:getPositionFromGridPosition(nextPosition)
		local positionTween			= self.transform:tweenJump(self:getPositionFromGridPosition(lastPosition), 1, 0.2)
		positionTween.playsBackward = true
		positionTween.progress		= 1

		local currentRotation	= self.transform.rotation
		self.transform.rotation = rotation
		local rotationTween		= self.transform:tweenRotation(currentRotation, 0.2, Ease.outQuad)
		rotationTween.playsBackward = true
		rotationTween.progress		= 1
	end
end

function Player:redoMove()

	-- there are no redo moves left for us so do nothing here
	if self.currentMoveIndex > #self.moves then
		print("No redos left")
		return
	end

	local move			  = self.moves[self.currentMoveIndex]
	self.currentMoveIndex = self.currentMoveIndex + 1
	move.redo()

	print("redoing move")
end

function Player:undoMove()

	-- there are no undo moves left for us so do nothing here
	if self.currentMoveIndex <= 1 then
		print("No undos left")
		return
	end

	self.currentMoveIndex = self.currentMoveIndex - 1
	if self.currentMoveIndex < 1 then
		self.currentMoveIndex = 1
	end

	local move = self.moves[self.currentMoveIndex]
	move.undo()
	self.level.undoCounts = self.level.undoCounts + 1

	print("undoing move")
end

function Player:start()

	self.transform = self.actor:get("Transform")
	local position = self.transform.position

	self.gridPosition.x = position.x + 1
	self.gridPosition.y = position.z + 1
	self.lastPosition.x = self.gridPosition.x
	self.lastPosition.y = self.gridPosition.y

	self.moves = {}
	self.currentMoveIndex = 1
	self.level.undoCounts = 0
	self.canControl = true
	self.pauseMenu.resumeButton:get("LuaBehavior").player = self
end

function Player:update()

	if Game.keyboard.isDown("escape") then

		Config.audio.ui.buttonPress:play()
		if self.level.isLevelComplete then
			return
		end

		self.canControl = not self.canControl
		self.pauseMenu:activate()
	end

	if not self.canControl then
		return
	end

	if Game.keyboard.isDown("r") then
		Game.loadScene(Config.startScene)
		return
	end

	if Game.keyboard.isHeld("LShift") or Game.keyboard.isHeld("RShift") then
		for key, value in pairs(inputKeys) do
			if Game.keyboard.isDown(key) then

				if not self.map:getDroppedKeysGridAt(self.gridPosition).hasKeyDropped[key] and not disabledKeys[key] then
					self:blockKey(key, true)
					return
				end

				if self.map:getDroppedKeysGridAt(self.gridPosition).hasKeyDropped[key] and disabledKeys[key] then
					self:unblockKey(key, true)
					return
				end
			end
		end
	end

	for key, value in pairs(inputKeys) do
		if not disabledKeys[key] and Game.keyboard.isDown(key) then
            Config.audio.ui.buttonPress:play()
			self:moveByKey(key)
			break
		end
	end
end

return function(o)
    return Player:new(o)
end