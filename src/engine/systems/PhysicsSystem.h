//
// Created by Maksym Maisak on 20/10/18.
//

#ifndef SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
#define SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H

#include <tuple>
#include <utility>
#include <chrono>
#include "System.h"
#include "Engine.h"
#include "glm.hpp"
#include "Collision.h"
#include "Actor.h"
#include "Average.h"
#include "SceneManager.h"
#include "Receiver.h"

namespace en {

    class Transform;
    struct Rigidbody;
    struct Collision;
    class Text;

    class PhysicsSystem : public System, Receiver<SceneManager::OnSceneClosed> {

    public:

        struct DiagnosticsInfo {

            utils::Average<std::chrono::nanoseconds> updateTimeAverage = {};
            std::chrono::nanoseconds updateTimeMin = std::chrono::nanoseconds::max();
            std::chrono::nanoseconds updateTimeMax = {};
        };

        struct UpdateInfo {

            std::chrono::nanoseconds time = {};
            int numCollisionChecks = 0;
            int numCollisions      = 0;
        };

        void update(float dt) override;
        PhysicsSystem& setGravity(const glm::vec3& gravity);

    private:

        void receive(const SceneManager::OnSceneClosed& info) override;

        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities);
        void addGravity(Entity entity, Transform& tf, Rigidbody& rb, float dt);

        void flushDiagnosticsInfo();
        Text& ensureDebugText();

        glm::vec3 m_gravity;
        std::vector<Collision> m_detectedCollisions;

        DiagnosticsInfo m_diagnosticsInfo;
        UpdateInfo m_currentUpdateInfo;
        Actor m_debugTextActor;
    };
}

#endif //SAXION_Y2Q1_CPP_PHYSICSSYSTEM_H
