#include <iostream>
#include <memory>

#include "Engine.h"
#include "TransformHierarchySystem.h"
#include "DestroySystem.h"
#include "DestroyByTimerSystem.h"
#include "RenderSystems.h"
#include "Render2DSystem.h"
#include "PhysicsSystem.h"
#include "PhysicsSystemBoundingSphereNarrowphase.h"
#include "PhysicsSystemFlatGrid.h"
#include "PhysicsSystemQuadtree.h"
#include "PhysicsSystemOctree.h"
#include "UIEventSystem.h"
#include "TweenSystem.h"

#include "PhysicsTestScene.h"
#include "PhysicsStressTestingSystem.h"
#include "CollisionDetectionTestScene.h"
#include "LightingScene.h"
#include "TerrainScene.h"
#include "AITestingScene.h"
#include "LuaScene.h"

#include "TestComponentPool.h"
#include "CameraControls2DSystem.h"

using namespace std::literals::chrono_literals;

void openStartLuaScene(en::Engine& engine) {

    en::LuaState& lua = engine.getLuaState();
    lua_getglobal(lua, "Config");
    const auto popConfig = lua::PopperOnDestruct(lua);
    const auto startScene = lua.tryGetField<std::string>("startScene");
    if (startScene) {
        engine.getSceneManager().setCurrentScene<en::LuaScene>(lua, *startScene);
    }
    lua.pop();
}

int main() {

    auto engine = std::make_unique<en::Engine>();
    engine->initialize();

    en::EngineSystems& systems = engine->getSystems();

    {
        systems.addSystem<en::TransformHierarchySystem>();
        systems.addSystem<en::RenderSystems>();

        //systems.addSystem<en::PhysicsStressTestingSystem>(10s);
        //systems.addSystem<en::PhysicsSystem>().setGravity({0, -9.8, 0});
        //systems.addSystem<en::PhysicsSystemBoundingSphereNarrowphase>().setGravity({0, -9.8, 0});
        //systems.addSystem<en::PhysicsSystemFlatGrid>().setGravity({0, -9.8, 0});
        //systems.addSystem<en::PhysicsSystemQuadtree>().setGravity({0, -9.8, 0});
        //systems.addSystem<en::PhysicsSystemOctree>().setGravity({0, -9.8, 0});

        systems.addSystem<CameraControls2DSystem>();

        systems.addSystem<en::UIEventSystem>();
        systems.addSystem<en::BehaviorsSystem>();

        systems.addSystem<en::TweenSystem>();
        systems.addSystem<en::DestroyByTimerSystem>();
        systems.addSystem<en::DestroySystem>();
    }

    //openStartLuaScene(*engine);
    engine->getSceneManager().setCurrentScene<AITestingScene>();
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>();
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{20, 0, glm::vec3(20.f)});
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{100, 100, glm::vec3(50.f)});
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{400, 1000, glm::vec3(50.f)});
    //engine->getSceneManager().setCurrentScene<en::CollisionDetectionTestScene>();
    //engine->getSceneManager().setCurrentScene<LightingScene>();
    //engine->getSceneManager().setCurrentScene<TerrainScene>();

    engine->run();

    return 0;
}
