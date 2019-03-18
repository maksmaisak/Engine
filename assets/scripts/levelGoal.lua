require('math')
require('assets/scripts/object')

LevelGoal = Object:new()

local function startFireFlickering(self)

    self.goal.light.actor:tweenComplete()
    local baselineIntensity = self.goal.light.initialIntensity
    local halfIntensityAmplitude = baselineIntensity * 0.6
    local minIntensity = baselineIntensity - halfIntensityAmplitude
    local maxIntensity = baselineIntensity + halfIntensityAmplitude
    self.goal.light.light.intensity = minIntensity
    self.goal.light.light:tweenIntensity(maxIntensity, 2, Ease.fluctuate):setLoopBounce()

    self.isFireFlickering = true
end

function LevelGoal:start()

    self.isFireActive = self.goal.startActive
    self.timeOfFireActivation = self.isFireActive and Game.getTime() or 0

    if self.isFireActive then
        startFireFlickering(self)
    else
        self.isFireFlickering = false
    end
end

function LevelGoal:update(dt)

    if self.isFireActive and not self.isFireFlickering then
        if Game.getTime() > self.timeOfFireActivation + 1 then
            startFireFlickering(self)
        end
    end
end

function LevelGoal:activateFire()

    if self.isFireActive then return end

    self.isFireActive = true
    self.timeOfFireActivation = Game.getTime()

    self.goal.light.actor:tweenKill()
    self.goal.light.light:tweenIntensity(self.goal.light.initialIntensity, 2, Ease.outExpo)
    Config.audio.levelExitFire.ignition:play()
    Config.audio.levelExitFire.continuous:play()
end

function LevelGoal:deactivateFire()

    if not self.isFireActive then return end

    self.isFireActive = false
    self.isFireFlickering = false

    self.goal.light.actor:tweenKill()
    self.goal.light.light:tweenIntensity(0, 0.8, Ease.outExpo)
    Config.audio.levelExitFire.ignition:stop()
    Config.audio.levelExitFire.continuous:stop()
end

return function(o)
    return LevelGoal:new(o)
end