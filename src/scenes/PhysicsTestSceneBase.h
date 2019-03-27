//
// Created by Maksym Maisak on 2019-03-26.
//

#ifndef ENGINE_PHYSICSTESTSCENEBASE_H
#define ENGINE_PHYSICSTESTSCENEBASE_H

#include <vector>
#include <memory>
#include <random>
#include "Model.h"
#include "RenderInfo.h"
#include "Rigidbody.h"
#include "Engine.h"
#include "Scene.h"

namespace en {

    // TODO make this used by the test scenes instead of be their base class. Composition over inheritance.
    class PhysicsTestSceneBase : public en::Scene {

    public:
        PhysicsTestSceneBase();

    protected:
        void setUpNonBodies(const glm::vec3& halfSize);
        void setUpBounds   (const glm::vec3& halfSize);

        Actor makeSphere(const glm::vec3& position, float radius = 1.f, bool isStatic = false);
        Actor makeAABB  (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(1.f), bool isStatic = false);
        Actor makeCube  (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(1.f), bool isStatic = false);

        glm::vec3 getRandomVectorMinMax(const glm::vec3& min, const glm::vec3& max);
        glm::vec3 getRandomVectorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize);
        std::default_random_engine m_randomEngine = std::default_random_engine(0);

    private:
        void cacheMaterials();
        std::shared_ptr<Material> getRandomBodyMaterial();

        std::shared_ptr<Model> m_sphereModel;
        std::shared_ptr<Model> m_cubeModel;
        std::vector<std::shared_ptr<Material>> m_materials;
        std::shared_ptr<Material> m_staticBodyMaterial;
        std::shared_ptr<Material> m_floorMaterial;
    };
}

#endif //ENGINE_PHYSICSTESTSCENEBASE_H
