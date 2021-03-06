//
// Created by Maksym Maisak on 2019-03-31.
//

#ifndef ENGINE_PHYSICSTESTBODYGENERATOR_H
#define ENGINE_PHYSICSTESTBODYGENERATOR_H

#include <vector>
#include <memory>
#include <random>
#include "Model.h"
#include "RenderInfo.h"
#include "Rigidbody.h"
#include "Engine.h"
#include "Scene.h"

namespace en {

    class PhysicsTestBodyGenerator {

    public:
        explicit PhysicsTestBodyGenerator(const glm::vec3& halfSize, const std::default_random_engine& randomEngine = std::default_random_engine(0));

        PhysicsTestBodyGenerator& setEngine(Engine& engine);

        bool getRandomizeRotation() const;
        PhysicsTestBodyGenerator& setRandomizeRotation(bool randomizeRotation);

        void setUpNonBodies();
        void setUpBounds   ();

        Actor makeSphere    (const glm::vec3& position, float radius = 1.f, bool isStatic = false);
        Actor makeAABB      (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(1.f), bool isStatic = false);
        Actor makeCube      (const glm::vec3& position, const glm::vec3& halfSize = glm::vec3(1.f), bool isStatic = false);
        Actor makeRandomBody(const glm::vec3& position, bool isStatic = false);

        std::default_random_engine& getRandomEngine();
        glm::vec3 getRandomBodyPosition();
        glm::quat getRandomBodyRotation();

        glm::vec3 getRandomVectorMinMax(const glm::vec3& min, const glm::vec3& max);
        glm::vec3 getRandomVectorCenterHalfSize(const glm::vec3& center, const glm::vec3& halfSize);

    private:

        void cacheMaterials();
        std::shared_ptr<Material> getRandomBodyMaterial();

        std::default_random_engine m_randomEngine;
        Engine* m_engine;
        glm::vec3 m_halfSize;
        bool m_randomizeRotation = false;

        std::shared_ptr<Model> m_sphereModel;
        std::shared_ptr<Model> m_cubeModel;
        std::vector<std::shared_ptr<Material>> m_materials;
        std::shared_ptr<Material> m_aabbMaterial;
        std::shared_ptr<Material> m_staticBodyMaterial;
        std::shared_ptr<Material> m_floorMaterial;
    };
}

#endif //ENGINE_PHYSICSTESTBODYGENERATOR_H
