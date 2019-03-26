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

    class PhysicsTestSceneBase : public en::Scene {

    public:
        PhysicsTestSceneBase();

    protected:
        void setUpNonBodies(const glm::vec3& halfSize);
        void setUpBounds   (const glm::vec3& halfSize);

        void makeSphere(const glm::vec3& position, float radius = 0.2f, bool isStatic = false);
        void makeCube  (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(0.4f), bool isStatic = false);

        glm::vec3 getRandomVectorMinMax(const glm::vec3& min, const glm::vec3& max);
        glm::vec3 getRandomVectorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize);
        std::default_random_engine m_randomEngine = std::default_random_engine(0);

    private:
        void cacheMaterials();

        std::shared_ptr<en::Model> m_sphereModel;
        std::shared_ptr<en::Model> m_cubeModel;
        std::vector<std::shared_ptr<en::Material>> m_materials;
        std::shared_ptr<en::Material> m_staticBodyMaterial;
        std::shared_ptr<en::Material> m_floorMaterial;
    };
}

#endif //ENGINE_PHYSICSTESTSCENEBASE_H
