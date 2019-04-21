#include <iostream>
#include <memory>

#include "Engine.h"
#include "Actor.h"
#include "TransformHierarchySystem.h"
#include "DestroySystem.h"
#include "DestroyByTimerSystem.h"
#include "RenderSystem.h"
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
#include "LuaScene.h"

#include "TestComponentPool.h"

using namespace std::literals::chrono_literals;

void openStartLuaScene(en::Engine& engine) {

    en::LuaState& lua = engine.getLuaState();
    lua_getglobal(lua, "Config");
    auto popConfig = lua::PopperOnDestruct(lua);
    auto startScene = lua.tryGetField<std::string>("startScene");
    if (startScene)
        engine.getSceneManager().setCurrentScene<en::LuaScene>(lua, *startScene);
    lua.pop();
}

int main() {

    std::cout << "Starting Game" << std::endl;

//    testIterationEntityRegistry();
//    testIterationHeapAllocated();

    auto engine = std::make_unique<en::Engine>();
    engine->initialize();
    {
        engine->addSystem<en::TransformHierarchySystem>();
        engine->addSystem<en::RenderSystem>();

        engine->addSystem<en::PhysicsStressTestingSystem>(10s);
        //engine->addSystem<en::PhysicsSystem>().setGravity({0, -9.8, 0});
        //engine->addSystem<en::PhysicsSystemBoundingSphereNarrowphase>().setGravity({0, -9.8, 0});
        //engine->addSystem<en::PhysicsSystemFlatGrid>().setGravity({0, -9.8, 0});
        //engine->addSystem<en::PhysicsSystemQuadtree>().setGravity({0, -9.8, 0});
        //engine->addSystem<en::PhysicsSystemOctree>().setGravity({0, -9.8, 0});

        engine->addSystem<en::UIEventSystem>();
        engine->addSystem<en::BehaviorsSystem>();

        engine->addSystem<en::TweenSystem>();
        engine->addSystem<en::DestroyByTimerSystem>();
        engine->addSystem<en::DestroySystem>();
    }

    //openStartLuaScene(*engine);
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>();
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{20, 0, glm::vec3(20.f)});
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{100, 100, glm::vec3(50.f)});
    //engine->getSceneManager().setCurrentScene<PhysicsTestScene>(PhysicsTestScene::Preset{500, 1000, glm::vec3(50.f)});
    //engine->getSceneManager().setCurrentScene<en::CollisionDetectionTestScene>();
    //engine->getSceneManager().setCurrentScene<LightingScene>();
    //engine->getSceneManager().setCurrentScene<TerrainScene>();

    engine->run();
    return 0;
}
