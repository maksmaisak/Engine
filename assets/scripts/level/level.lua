require('assets/scripts/object')
require('assets/scripts/vector')
require('assets/scripts/level/map')
require('assets/scripts/UI/pauseMenu')
require('assets/scripts/UI/redoUndoButtons')
require('assets/scripts/UI/hudArrows')
require('assets/scripts/UI/tutorialScreen')

Level = Object:new {
	map = nil,
	nextLevelPath = nil
}

local function doesAnyGoalStartActive(map)

	for x = 1, map:getGridSize().x do
		for y = 1, map:getGridSize().y do
			local gridItem = map:getGridAt({x = x, y = y})
			if gridItem and gridItem.goal and gridItem.goal.startActive then
				return true
			end
		end
	end

	return false
end

function Level:start()

	if not self.map then
		print('Level: no map')
		return
	end

	if Config.overlayPhiGrid then
		Game.makeActor {
			Transform = {},
			UIRect = {},
			Sprite = {material = {shader = "sprite", texture = "textures/phiGrid.png", color = {1,1,1,0.5}}}
		}
	end

	if doesAnyGoalStartActive(self.map) then
		Config.audio.levelExitFire.continuous:play()
	else
		Config.audio.levelExitFire.continuous:stop()
	end

	local music = Config.audio.ambience
	if music.status ~= "Playing" then
		music:play()
	end

	self.redoUndoButtons = RedoUndoButtons:new {level = self}
	self.hudArrows = HudArrows:new {level = self}
	self.pauseMenu = PauseMenu:new {level = self}

    for x = 1, self.map:getGridSize().x do
        for y = 1, self.map:getGridSize().y do

			local gridItem = self.map:getGridAt({x = x, y = y})

			if gridItem.tile then
				gridItem.tile = Game.makeActor(gridItem.tile)
			end

			if gridItem.obstacle then
				gridItem.obstacle = Game.makeActor(gridItem.obstacle)
			end

			if gridItem.player then
				gridItem.player = Game.makeActor(gridItem.player)
				gridItem.player:add("LuaBehavior", dofile(Config.player) {
					level = self,
					map   = self.map,
					pauseMenu  = self.pauseMenu,
				})
				self.player = gridItem.player:get("LuaBehavior")
			end

			if gridItem.goal then

                local goal		= gridItem.goal
                goal.actor		= Game.makeActor(goal.actor)
				goal.transform	= goal.actor:get("Transform")

				if goal.light then

					goal.light = {actor = Game.makeActor(goal.light) }
					local light = goal.light
					light.transform = goal.light.actor:get("Transform")
					light.transform.parent	= goal.actor
					light.light = goal.light.actor:get("Light")
					light.initialIntensity = goal.light.light.intensity
					if not goal.startActive then
						light.light.intensity = 0
					end
				end

				local behavior = goal.actor:add("LuaBehavior", require('assets/scripts/levelGoal'){goal = goal})
				goal.activateFire   = function() behavior:activateFire()   end
				goal.deactivateFire = function() behavior:deactivateFire() end
			end

			if gridItem.button then

				local button	 = gridItem.button
				button.actor	 = Game.makeActor(button.actor)
				button.transform = button.actor:get("Transform")
			end

			if gridItem.portal then
				gridItem.portal.actor = Game.makeActor(gridItem.portal.actor)
			end

			if gridItem.door then

				local door	   = gridItem.door
				door.actor	   = Game.makeActor(door.actor)
				door.transform = door.actor:get("Transform")

				door.swingLeft					= Game.makeActor(door.swingLeft)
				door.swingLeftTransform			= door.swingLeft:get("Transform")
				door.swingLeftTransform.parent	= door.actor

				door.swingRight					= Game.makeActor(door.swingRight)
				door.swingRightTransform		= door.swingRight:get("Transform")
				door.swingRightTransform.parent = door.actor
			end

			if gridItem.laser then

				local laser = gridItem.laser

				laser.actor = Game.makeActor(laser.actor)
				laser.beam  = Game.makeActor(laser.beam)
				laser.beam:get("Transform").parent = laser.actor

				laser.actor:add("LuaBehavior", require('assets/scripts/laser') {
					level		 = self,
					map			 = self.map,
					direction	 = {x = laser.direction.x, y = laser.direction.y},
					gridPosition = {x = x, y = y}
				})

				self.map.lasers[#self.map.lasers + 1] = laser
			end
        end
	end

	if self.extras then
		Game.makeActors(self.extras)
	end

	self.tutorialScreen = TutorialScreen:new {level = self}
end