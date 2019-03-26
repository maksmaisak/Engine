//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef ENGINE_PHYSICSTESTSCENE_H
#define ENGINE_PHYSICSTESTSCENE_H

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

    void cacheMaterials();

    void setUpNonBodies();
    void setUpBounds();

    void addStaticBodies();
    void addDynamicBodies();

    void makeSphere(const glm::vec3& position, float radius = 0.2f, bool isStatic = false);
    void makeCube  (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(0.4f), bool isStatic = false);

    Preset m_preset;
    std::default_random_engine m_randomEngine = std::default_random_engine(0);
    std::function<glm::vec3()> m_randomPosition;

    std::shared_ptr<en::Model> m_sphereModel;
    std::shared_ptr<en::Model> m_cubeModel;
    std::vector<std::shared_ptr<en::Material>> m_materials;
    std::shared_ptr<en::Material> m_staticBodyMaterial;
    std::shared_ptr<en::Material> m_floorMaterial;
};


#endif //ENGINE_PHYSICSTESTSCENE_H
