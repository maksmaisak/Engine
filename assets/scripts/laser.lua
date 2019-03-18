require('math')
require('assets/scripts/object')
require('assets/scripts/vector')
require('assets/scripts/level/map')
require('assets/scripts/player')

Laser = Object:new {
	level = nil,
	map   = nil
}

function Laser:start()

	self.timer		   = 0
	self.checkInterval = 1
	self.gridItem	   = self.map:getGridAt(self.gridPosition)
	self.transform	   = self.actor:get("Transform")

	self.beamRenderInfo = self.gridItem.laser.beam:getInChildren("RenderInfo")
	self.beamTransform  = self.gridItem.laser.beam:get("Transform")

	self:updateBeamLength()
end

function Laser:update(dt)

	if self.beamRenderInfo then
		self.beamRenderInfo.isEnabled = self.gridItem.isButtonTargetEnabled or false
	end

	self.timer = self.timer + dt
	if self.gridItem.isButtonTargetEnabled then
		if self.timer >= self.checkInterval then
			self:hitDroppedKeys()
			self.timer = 0
		end
	end
end

local function doesBlock(gridItem)
	return not gridItem or gridItem.obstacle or (gridItem.door and not gridItem.isButtonTargetEnabled)
end

function Laser:hitDroppedKeys()
	if not self.level.player.canControl then
		return
	end

	print("hitting dropped keys")

	local position = {x = self.gridPosition.x, y = self.gridPosition.y}
	for i = 0, math.max(self.map.gridSize.x, self.map.gridSize.y) do

		local gridItem = self.map:getGridAt(position)
		if i > 0 and doesBlock(gridItem) then
			self:setBeamLength(i)
			return
		end

		for k, v in pairs(self.map:getDroppedKeysGridAt(position).hasKeyDropped) do
			if v then -- if has this currentKey dropped at this position
				self.level.player:moveByKey(k)
			end
		end

		position.x = position.x + self.direction.x
		position.y = position.y + self.direction.y

		if position.x > self.map.gridSize.x or position.x < 1 then
			break
		end

		if position.y > self.map.gridSize.y or position.y < 1 then
			break
		end
	end
end

function Laser:updateBeamLength()

	local position = {x = self.gridPosition.x, y = self.gridPosition.y}
	for i = 0, math.max(self.map.gridSize.x, self.map.gridSize.y) do

		local gridItem = self.map:getGridAt(position)
		if i > 0 and doesBlock(gridItem) then
			self:setBeamLength(i)
			return
		end

		position.x = position.x + self.direction.x
		position.y = position.y + self.direction.y

		if position.x > self.map.gridSize.x or position.x < 1 then
			break
		end

		if position.y > self.map.gridSize.y or position.y < 1 then
			break
		end
	end
end

function Laser:setBeamLength(length)

	local scale = self.beamTransform.scale
	scale.z = length
	self.beamTransform.scale = scale
end

return function(o)
    return Laser:new(o)
end