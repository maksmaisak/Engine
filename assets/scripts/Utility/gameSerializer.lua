require('assets/scripts/object')

GameSerializer = Object:new()

local function fileExists(file)

	local f = io.open(file, "rb")
	if f then f:close() end
	return f ~= nil
end

-- get all lines from a file, returns an empty 
-- list/table if the file does not exist
function GameSerializer:loadLines(file)

	if not fileExists(file) then
		return
	end

	local lines = {}

	for line in io.lines(file) do 
	  lines[#lines + 1] = line
	end

	return lines
end

local function split(str, delim, maxNb)

	-- Eliminate bad cases...
	if string.find(str, delim) == nil then
	   return { str }
	end
	
	if maxNb == nil or maxNb < 1 then
	   maxNb = 0    -- No limit
	end

	local result = {}
	local pat	 = "(.-)" .. delim .. "()"
	local nb	 = 0
	local lastPos

	for part, pos in string.gmatch(str, pat) do

	   nb			= nb + 1
	   result[nb]	= part
	   lastPos		= pos

	   if nb == maxNb then
	      break
	   end
	end

	-- Handle the last field
	if nb ~= maxNb then
	   result[nb + 1] = string.sub(str, lastPos)
	end
	return result
end

function GameSerializer:load(fileName)

	local lines = self:loadLines(fileName)

	if lines == {} then
		return {}
	end

	local data = {}

	for i = 1, #lines do

		local line		 = lines[i]
		local tempArray  = split(line, ", ")
		local savedEntry = {level = "", stars = 0}

		for i = 1, #tempArray do
			local valueArray = split(tempArray[i], ": ")
			local value		 = valueArray[2]

			if i == 1 then
				savedEntry.level = value
			elseif i == 2 then
				savedEntry.stars = tonumber(value)
			end
		end

		data[#data + 1] = savedEntry
	end

	for k, v in ipairs(data) do
		print("Loaded Data: Level: "..v.level.." Stars: "..v.stars)
	end

	return data
end

function GameSerializer:saveNewEntry(newEntry)
	local found = false

	for k, v in ipairs(Game.savedData) do
		if v.level == newEntry.level then
			if v.stars < newEntry.stars then
				Game.savedData[k].stars = newEntry.stars
			end
			found = true
			break
		end
	end

	if not found then
		if Game.savedData == nil then
			Game.savedData = { }
		end

		Game.savedData[#Game.savedData + 1] = newEntry
	end

	self:save(Config.saveFile)
end

function GameSerializer:save(fileName)
	if fileName == nil then
		return
	end

	local file = io.open(fileName, "w+")
	io.output(file)

	local savedData = Game.savedData
	if not savedData == nil then
		return
	end

	for k ,v in ipairs(savedData) do
		print("Saved data: Level: "..v.level.." Stars: "..v.stars)
	    io.write("level: "..v.level..", stars: "..v.stars.."\n")
	end

	file:close()
end