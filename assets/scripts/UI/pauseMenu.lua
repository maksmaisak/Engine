require('assets/scripts/object')
require('assets/scripts/Utility/uiUtilities')

PauseMenu = Object:new()

function PauseMenu:init()	
	self:createPanel()
	self.pauseMenuPanel:get("UIRect").isEnabled = false
end

function PauseMenu:createPanel()	
	
	self.pauseMenuPanel = Game.makeActor {
		Name = "PauseMenuPanel",
		Transform = {
			scale  = {1,1,1}
		},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {1, 1}
		}
	}

	local titleText = "PAUSED"

	Game.makeActor {
		Name = "PauseTextBackdrop",
		Transform = {
			scale = {1,1,1},
			parent = self.pauseMenuPanel
		},
		UIRect = {
			anchorMin = {0.002, 0.002},
			anchorMax = {1.002, 1.5002}
		},
		Text = {
			font = "fonts/arcadianRunes.ttf",
			fontSize = 120,
			color = {25/255, 14/255, 4/255, 0.8},
			string = titleText
        }
	}

	Game.makeActor {
		Name = "PauseText",
		Transform = {
			scale = {1,1,1},
			parent = self.pauseMenuPanel
		},
		UIRect = {
			anchorMin = {0, 0},
			anchorMax = {1, 1.5}
		},
		Text = {
			font = "fonts/arcadianRunes.ttf",
			fontSize = 120,
			color = {168/255, 130/255, 97/255, 1},
			string = titleText
        }
	}

	self.resumeButton = UIUtilities.makeButton("ResumeButton", self.pauseMenuPanel, "Resume", {0.5, 0.6}, {0.5, 0.6}, "textures/buttonBackground.png", function()
		self.pauseMenuPanel:get("UIRect").isEnabled = false
		self.level.player.canControl = true
	end)

	local mainMenuButton = UIUtilities.makeButton("MainMenuButton", self.pauseMenuPanel, "Main Menu", {0.5, 0.5}, {0.5, 0.5}, "textures/buttonBackground.png", function()
		Game.loadScene(Config.startScene)
	end)

	UIUtilities.keepAspectRatio(mainMenuButton   , 64)
	UIUtilities.keepAspectRatio(self.resumeButton, 64)
end

function PauseMenu:activate()

	local rect = self.pauseMenuPanel:get("UIRect")
	rect.isEnabled = not rect.isEnabled
end