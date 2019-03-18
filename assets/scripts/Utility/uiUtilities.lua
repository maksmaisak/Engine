--
-- User: maks
-- Date: 2019-03-14
-- Time: 12:13
--

UIUtilities = {}

function UIUtilities.setWidthBasedOnHeight(uiRect, aspect)

    local width = uiRect.computedSize.y * aspect
    local scaleFactorCompensator = 1 / Game.getUIScaleFactor()

    local offsetMin = uiRect.offsetMin
    local offsetMax = uiRect.offsetMax
    offsetMin.x = -width * 0.5 * scaleFactorCompensator
    offsetMax.x =  width * 0.5 * scaleFactorCompensator
    uiRect.offsetMin = offsetMin
    uiRect.offsetMax = offsetMax
end

function UIUtilities.keepAspectRatio(actor, theight)

    local sprite = actor:get("Sprite")
    if not sprite then return end

    local textureSize = sprite.textureSize
    local height = theight
    local width = height * textureSize.x / textureSize.y

    local rect = actor:get("UIRect")
    rect.offsetMin = { width * -0.5, height * -0.5 }
    rect.offsetMax = { width *  0.5, height *  0.5 }
end

function UIUtilities.makeButton(name, parent, textString, anchorMin, anchorMax, textureFilePath, onClick, conditionForActive)

    local buttonActor = Game.makeActor {
        Name = name,
        Transform = { parent = parent },
        Text = {
            font     = "fonts/arcadianRunes.ttf",
            fontSize = 36,
            color    = {0,0,0,1},
            string   = textString
        },
        UIRect = {
            anchorMin = anchorMin,
            anchorMax = anchorMax,
        },
        Sprite = {
            material = {
                shader	= "sprite",
                texture = textureFilePath
            }
        },
        LuaBehavior = {
            start = function(self)
                self.transform = self.actor:get("Transform")
                self.uiRect    = self.actor:get("UIRect")
                self.wasActive = not conditionForActive or conditionForActive()

                self.wasMouseOverWhileActive = false
            end,
            update = function(self, dt)

                local isActive = not conditionForActive or conditionForActive()

                local isMouseOverWhileActive = isActive and self.uiRect.isMouseOver

                if isMouseOverWhileActive and not self.wasMouseOverWhileActive then

                    self.actor:tweenKill()
                    self.transform:tweenScale({1.2, 1.2, 1.2}, 0.05)

                elseif not isMouseOverWhileActive and self.wasMouseOverWhileActive then

                    self.actor:tweenKill()
                    self.transform:tweenScale({1, 1, 1}, 0.05)
                end

                self.wasMouseOverWhileActive = isMouseOverWhileActive
            end,
            onMouseDown = function(self, button)

                if conditionForActive and not conditionForActive() then return end

                if button == 1 then

                    Config.audio.ui.buttonPress:play()
                    if onClick then
                        onClick(self)
                    end
                end
            end,
        }
    }

    UIUtilities.keepAspectRatio(buttonActor, 55)

    return buttonActor
end

