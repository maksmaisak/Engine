require('assets/scripts/level/level')
require('assets/scripts/Utility/gameSerializer')
require('assets/scripts/Utility/uiUtilities')

local isBlocked = false
local isChooseLevelOpened
local mainMenuPanel
local chooseLevelPanel
local chooseLevelImage
local creditsPanel

local levelIndex = 1
local buttonSize = 60

local stars = {}
local ratio

local scenery = {
	{
		Name = "Camera",
		Transform = {
			position = { x = 0, y = 0, z = 10 }
		},
		Camera = {},
	}
}

local scene = {}

local function canPlayLevel(levelIndex)

	if levelIndex == 1 then return true end
	return (Game.savedData[levelIndex - 1] or Game.savedData[levelIndex]) ~= nil
end

local function createStar(aMinX,aMinY,aMaxX,aMaxY)
	return Game.makeActor{
		Name = "Star",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {aMinX, aMinY},
			anchorMax = {aMaxX, aMaxY}
		},
		Sprite = {
			material = {
				shader	= "sprite",
				texture	= "textures/star.png",
			}
		}
	}
end

function scene:start()

    Config.audio.ambience:stop()
    Config.audio.levelExitFire.continuous:stop()

	if Config.overlayPhiGrid then
		Game.makeActor {
			Transform = {},
			UIRect = {},
			Sprite = {material = {shader = "sprite", texture = "textures/phiGrid.png", color = {1,1,1,0.5}}}
		}
	end

	Game.makeActors(scenery)

	-- load saved data
	local serializer = GameSerializer:new()
	Game.serializer  = serializer
	Game.savedData   = serializer:load(Config.saveFile)

	Game.levels   = dofile(Config.levelsDefinition)
	Game.maxLevel = #Game.levels

-- Start Main Buttons Panel
   	mainMenuPanel = Game.makeActor {
		Name = "MainMenuPanel",
		Transform = {},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {1, 1}
		}
	}

	UIUtilities.makeButton("StartButton", "MainMenuPanel", "Start", {0.5,0.7}, {0.5,0.7}, "textures/buttonBackground.png", function(self)
		Game.currentLevel = 1
		Game.loadScene(Game.levels[Game.currentLevel].path)
	end)

	UIUtilities.makeButton("ChooseLevelButton", "MainMenuPanel", "Choose Level", {0.5,0.6}, {0.5,0.6}, "textures/buttonBackground.png", function(self)
		isChooseLevelOpened = true
		chooseLevelPanel:get("UIRect").isEnabled = true
		mainMenuPanel   :get("UIRect").isEnabled = false
	end)

	UIUtilities.makeButton("CreditsButton", "MainMenuPanel", "Credits", {0.5,0.5}, {0.5,0.5}, "textures/buttonBackground.png", function(self)
		creditsPanel :get("UIRect").isEnabled = true
		mainMenuPanel:get("UIRect").isEnabled = false
	end)

	UIUtilities.makeButton("ExitButton", "MainMenuPanel", "Exit", {0.5,0.4}, {0.5,0.4}, "textures/buttonBackground.png", Game.quit)
--End Main Buttons Panel

--Start ChooseLevel Panel

	chooseLevelPanel = Game.makeActor {
		Name = "ChooseLevelPanel",
		Transform = {},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {1, 1}
		},		
		Sprite = {
			material = {
				shader	= "sprite",
				texture = "textures/transparent.png"
			}
		},
	}
	
	local arrowLeft = Game.makeActor {
		Name = "ArrowLeft",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {0.05, 0.6},
			anchorMax = {0.05, 0.6}
		},
		Sprite = {
			material = {
				shader	= "sprite",
				texture = "textures/arrowLeft.png"
			}
		},
		LuaBehavior = {
			update = function(self, dt)
				if Game.keyboard.isHeld("left") or (Game.mouse.isHeld(1) and self.actor:get("UIRect").isMouseOver) then
					self.actor:get("Transform").scale = {1.2,1.2,1.2}
				else
					self.actor:get("Transform").scale = {1,1,1}
				end
			end
		}
	}

	local arrowRight = Game.makeActor {
		Name = "ArrowRight",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {0.95, 0.6},
			anchorMax = {0.95, 0.6}
		},
		Sprite = {
			material = {
				shader	= "sprite",
				texture = "textures/arrowRight.png"
			}
		},
		LuaBehavior = {
			update = function(self, dt)
				if Game.keyboard.isHeld("right") or (Game.mouse.isHeld(1) and self.actor:get("UIRect").isMouseOver) then
					self.actor:get("Transform").scale = {1.2,1.2,1.2}
				else
					self.actor:get("Transform").scale = {1,1,1}
				end
			end
		}
	}

	local textActorBackground = Game.makeActor {
		Name = "ChooseLevelImageTextBackground",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {0.502, 0.802},
			anchorMax = {0.502, 0.802}
		},
		Text = {
			font     = "fonts/arcadianRunes.ttf",
			fontSize = 50,
			color    = {25/255,14/255,4/255,1},
			string   = "Level : "..levelIndex
        },
	}

	local textActor = Game.makeActor {
		Name = "ChooseLevelImageText",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {0.5, 0.8},
			anchorMax = {0.5, 0.8}
		},
		Text = {
			font     = "fonts/arcadianRunes.ttf",
			fontSize = 50,
			color    = {168/255,130/255,97/255,1},
			string   = "Level : "..levelIndex
        },
	}

	chooseLevelImage = Game.makeActor {
		Name = "ChooseLevelImage",
		Transform = {
			parent = "ChooseLevelPanel"
		},
		UIRect = {
			anchorMin = {0.5, 0.55},
			anchorMax = {0.5, 0.55}
		},
		Sprite = {material = {shader = "sprite"}},
		LuaBehavior = {
			start = function(self)
				self.isFirstUpdate = true
				self.uiRect = self.actor:get("UIRect")
			end,
			update = function(self, dt)

				if not isChooseLevelOpened then
					return
				end

				local isLevelChanged = false

				if self.isFirstUpdate then
					self.isFirstUpdate = false
					isLevelChanged = true
				end

				local function updateTitleText()

					local text = "Level "..levelIndex
					textActor:get("Text").string = text
					textActorBackground:get("Text").string = text
				end

				if levelIndex < Game.maxLevel then
					if Game.keyboard.isDown("right") or (Game.mouse.isDown(1) and arrowRight:get("UIRect").isMouseOver) then

						Config.audio.ui.buttonPress:play()

						levelIndex = levelIndex + 1

						updateTitleText();

						if levelIndex > Game.maxLevel then
							levelIndex = Game.maxLevel
						end

						if levelIndex >= Game.maxLevel then
							arrowRight:get("UIRect").isEnabled = false
						end

						if levelIndex >= 1 then
							arrowLeft:get("UIRect").isEnabled = true
						end

						isLevelChanged = true
					end
				end

				if levelIndex > 1 then
					if Game.keyboard.isDown("left") or (Game.mouse.isDown(1) and arrowLeft:get("UIRect").isMouseOver) then

						Config.audio.ui.buttonPress:play()

						levelIndex = levelIndex - 1

						updateTitleText();

						if levelIndex < 1 then
							levelIndex = 1
						end

						if levelIndex <= 1 then
							arrowLeft:get("UIRect").isEnabled = false
						end

						if levelIndex < Game.maxLevel then
							arrowRight:get("UIRect").isEnabled = true
						end

						isLevelChanged = true
					end
				end

				if isLevelChanged then

					isLevelChanged = false

					-- destroy all the stars from previous level before creating
					-- the new ones
					for k, v in ipairs(stars) do
						if v ~= nil then
							v:remove("Sprite")
							v = nil
						end
					end

					local totalNumberOfStars = 0
					local savedLevelData = Game.savedData[levelIndex]
					if savedLevelData then
						totalNumberOfStars = savedLevelData.stars
					end

					self.actor:remove("Sprite")
					local level = Game.levels[levelIndex]
					if level then
						local imagePath = level.thumbnailPath
						local sprite = self.actor:add("Sprite", {
							material = {
								shader	= "sprite",
								texture = imagePath,
								color   = canPlayLevel(levelIndex) and {1,1,1,1} or {0,0,0,1}
							}
						})

						local textureSize = sprite.textureSize
						UIUtilities.setWidthBasedOnHeight(self.uiRect, textureSize.x / textureSize.y)
					end

					local anchorValue = 0.4
					local anchorStep  = 0.1

					for i = 1, totalNumberOfStars do
						self.starActor = createStar(anchorValue, 0.3, anchorValue, 0.3)
						stars[#stars + 1] = self.starActor
						UIUtilities.keepAspectRatio(self.starActor, 100)
						anchorValue = anchorValue + anchorStep
					end
				end

				if canPlayLevel(levelIndex) then
					if Game.keyboard.isDown("enter") or (Game.mouse.isDown(1) and self.uiRect.isMouseOver) then
						Config.audio.ui.buttonPress:play()
						Game.loadScene(Game.levels[levelIndex].path)
						print("Loaded scene : "..levelIndex)
						Game.currentLevel = levelIndex
					end
				end
			end
		}
	}

	UIUtilities.makeButton("BackButtonChooseLevel", "ChooseLevelPanel", "Back", {0.5, 0.15}, {0.5, 0.15}, "textures/buttonBackground.png", function(self)
		isChooseLevelOpened = false
		chooseLevelPanel:get("UIRect").isEnabled = false
		mainMenuPanel   :get("UIRect").isEnabled = true
	end)

--End ChooseLevel Panel

--Start Credits Panel

	creditsPanel = Game.makeActor {
		Name = "CreditsPanel",
		Transform = {},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {1, 1}
		}
	}

	local text = "        Credits\n\nEngineers\n\nMaks Maisak\nCosmin Bararu\nGeorge Popa\n\nDesigners\n\nKateryna Malyk\nGustav Eckrodt\nEmre Hamazkaya\nLea Kemper\nYuchen Bao"

	Game.makeActor {
		Name = "CreditsText",
		Transform = {
			parent = "CreditsPanel",
		},
		UIRect = {
			anchorMin = {0.01, 0.05},
			anchorMax = {1, 1}
		},
		Text = {
			font = "fonts/arcadianRunes.ttf",
			fontSize = 30,
			color = Config.textColors.primary,
			string = text
		}
	}

	Game.makeActor {
		Name = "CreditsTextBackround",
		Transform = {
			parent = "CreditsPanel",
		},
		UIRect = {
			anchorMin = {0.01002, 0.05002},
			anchorMax = {1.002, 1.002}
		},
		Text = {
			font   = "fonts/arcadianRunes.ttf",
			fontSize = 30,
			color = Config.textColors.backdrop,
			string = text
		}
	}

	UIUtilities.makeButton("BackButtonCredits", "CreditsPanel","Back", {0.5,0.15}, {0.5,0.15}, "textures/buttonBackground.png", function(self)
		creditsPanel :get("UIRect").isEnabled = false
		mainMenuPanel:get("UIRect").isEnabled = true
	end)

--End Credits Panel

	local menuBackground = Game.makeActor {
		Name = "MainPanelImage",
		Transform = {},
		UIRect = {
			anchorMin = {0.5, 0},
			anchorMax = {0.5, 1}
		},
		Sprite = {
			material = {
				shader	= "sprite",
				texture	= "textures/menuBackground.jpg"
			}
		},
		LuaBehavior = {
			start = function(self)
				self.sprite = self.actor:get("Sprite")
				self.uiRect = self.actor:get("UIRect")
			end,
			update = function(self)
				local textureSize = self.sprite.textureSize
				local aspect = textureSize.x / textureSize.y
				UIUtilities.setWidthBasedOnHeight(self.uiRect, aspect)
			end
		}
	}

	creditsPanel:get("UIRect").isEnabled     = false
	chooseLevelPanel:get("UIRect").isEnabled = false
	arrowLeft:get("UIRect").isEnabled        = false

	UIUtilities.keepAspectRatio(chooseLevelImage, 400)
	UIUtilities.keepAspectRatio(arrowLeft , 300)
	UIUtilities.keepAspectRatio(arrowRight, 300)
end

return scene
