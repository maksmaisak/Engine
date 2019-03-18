require('assets/scripts/object')

Map = Object:new {
	grid			= {},
	gridSize		= { x = 10, y = 10 },
	droppedKeysGrid = {},
	lasers = {}
}

function Map:init()

	for x = 1, self.gridSize.x do
		self.grid[x] = {}
		for y = 1, self.gridSize.y do
			self.grid[x][y] = {}
		end
	end

	for x = 1, self.gridSize.x do
		self.droppedKeysGrid[x] = {}
		for y = 1, self.gridSize.y do
			self.droppedKeysGrid[x][y]				 = {}
			self.droppedKeysGrid[x][y].keys			 = {up = nil, down = nil, left = nil, right = nil }
			self.droppedKeysGrid[x][y].hasKeyDropped = {up = false, down = false, left = false, right = false}
		end
	end
end

function Map:getGridSize()
	return self.gridSize
end

function Map:getGridAt(gridPosition)
	return self.grid[gridPosition.x][gridPosition.y]
end

function Map:getDroppedKeysGridAt(gridPosition)
	return self.droppedKeysGrid[gridPosition.x][gridPosition.y]
end

function Map:calculateYOffset(gridPosition)

	local yOffset = 0.0

	for k, v in pairs(self:getDroppedKeysGridAt(gridPosition).keys) do
		for m, p in pairs(v) do
			if p then
				yOffset = yOffset + p:get("Transform").scale.y * 0.2
			end
		end
	end

	return yOffset
end

function Map:makeGameObject(gridPosition, objectData)
    local xNorm = (gridPosition.x - 1) / self.gridSize.x
    local yNorm = (gridPosition.y - 1) / self.gridSize.y
    local position = {
        x = (xNorm - 0.5) * self.gridSize.x,
        y = objectData.startYPosition,
        z = (yNorm - 0.5) * self.gridSize.y
    }

    return Game.makeActor {
        Name = objectData.name..gridPosition.x.."."..gridPosition.y,
        Transform = {
            position	= position,
            scale		= objectData.scale
        },
        RenderInfo = {
            mesh		= objectData.mesh,
            material	= objectData.material
        }
    }
end

