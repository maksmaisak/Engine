
-- engine
width  = 1920
height = 1080
windowTitle = "Sealed"
fullscreen = false
framerateCap = 240
vsync = false
enableStaticBatching = true
enableDebugOutput    = false
overlayPhiGrid       = false
referenceResolution = {1920, 1080}
startScene = 'assets/scripts/UI/mainMenu.lua'
--startScene = 'assets/scripts/scenes/level1.lua'
--startScene = 'assets/scripts/scenes/test/luaScene.lua'
--startScene = 'assets/scripts/scenes/test/testTweening.lua'
--startScene = 'assets/scripts/scenes/test/renderingShowcase.lua'

defaultSkybox = {
    right  = "textures/skybox/SkyboxMap_right.png" ,
    left   = "textures/skybox/SkyboxMap_left.png"  ,
    top    = "textures/skybox/SkyboxMap_top.png"   ,
    bottom = "textures/skybox/SkyboxMap_bottom.png",
    front  = "textures/skybox/SkyboxMap_front.png" ,
    back   = "textures/skybox/SkyboxMap_back.png"
}

-- custom
player = 'assets/scripts/player.lua'
resultScreen = 'assets/scripts/UI/resultScreen.lua'
saveFile = 'assets/scripts/save.lua'
levelsDefinition = 'assets/scripts/levelsDefinition.lua'
textColors = {
    primary  = {168/255, 130/255, 97/255, 1},
    backdrop = {25/255, 14/255, 4/255, 0.8}
}

local function makeSound(path, volume, loop, offset)

    local sound = Game.audio.makeSound(path)
    if not sound then return nil end

    sound.volume = volume or 100
    sound.loop   = loop or false
    sound.playingOffset = offset and sound.duration * offset or 0
    return sound
end

local function makeMusic(path, volume, loop, offset)

    local music = Game.audio.makeMusic(path)
    if not music then return nil end

    music.volume = volume or 100
    music.loop   = loop or false
    music.playingOffset = offset and music.duration * offset or 0
    return music
end

audio = {
    ambience = makeMusic('audio/Ambience.wav', 100, true),
    door = {
        open  = makeSound('audio/doorOpen.wav' , 20),
        close = makeSound('audio/doorClose.wav', 20),
    },
    portal = makeSound('audio/Portal.wav'),
    levelFinished = makeSound('audio/doorOpen.wav', 60),
    levelExitFire = {
        ignition   = makeSound('audio/levelExit/FireStart.wav'   , 20),
        continuous = makeMusic('audio/levelExit/FireLoopable.wav', 20, true)
    },
    ui = {
        buttonPress = makeSound('audio/UIButtonSound.wav', 20),
        stars = {
            makeSound('audio/stars/1Star.wav', 50),
            makeSound('audio/stars/2Star.wav', 50),
            makeSound('audio/stars/3Star.wav', 50)
        }
    },
}