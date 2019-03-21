//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef SAXION_Y2Q2_RENDERING_PHYSICSTESTSCENE_H
#define SAXION_Y2Q2_RENDERING_PHYSICSTESTSCENE_H

#include <vector>
#include <memory>
#include <random>
#include "Model.h"
#include "RenderInfo.h"
#include "Rigidbody.h"
#include "Engine.h"
#include "Scene.h"

class PhysicsTestScene : public en::Scene {

public:
    struct Preset {
        int numBodiesStatic  = 100;
        int numBodiesDynamic = 100;
        glm::vec3 fieldHalfSize = glm::vec3(50);
    };

    // Clang bug doesn't allow to use {} for the default member initializers
    PhysicsTestScene(const Preset& preset = {100, 100, glm::vec3(50)});
    void open() override;
    void update(float dt) override;

private:

    void cacheRenderInfos();

    void setUpNonBodies();
    void setUpBounds();

    void addStaticBodies();
    void addDynamicBodies();

    void makeSphere(const glm::vec3& position, float radius = 0.2f, bool isStatic = false);
    void makeCube  (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(0.4f), bool isStatic = false);

    Preset m_preset;
    std::default_random_engine m_randomEngine = std::default_random_engine(0);
    std::function<glm::vec3()> m_randomPosition;

    en::RenderInfo m_floorRenderInfo;
    en::RenderInfo m_sphereRenderInfo;
    en::RenderInfo m_cubeRenderInfo;
};


#endif //SAXION_Y2Q2_RENDERING_PHYSICSTESTSCENE_H
