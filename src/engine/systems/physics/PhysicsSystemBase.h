//
// Created by Maksym Maisak on 2019-04-01.
//

#ifndef ENGINE_PHYSICSSYSTEMBASE_H
#define ENGINE_PHYSICSSYSTEMBASE_H

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

    class PhysicsSystemBase : public System {

    public:

        struct DiagnosticsInfo {

            utils::Average<std::chrono::nanoseconds> updateTimeAverage = {};
            std::chrono::nanoseconds updateTimeMin = std::chrono::nanoseconds::max();
            std::chrono::nanoseconds updateTimeMax = {};
            std::vector<std::chrono::nanoseconds> updateTimes;
        };

        struct UpdateInfo {

            std::chrono::nanoseconds time = {};
            int numCollisionChecks = 0;
            int numCollisions      = 0;
        };

        PhysicsSystemBase& setGravity(const glm::vec3& gravity);
        const DiagnosticsInfo& getDiagnosticsInfo() const;
        DiagnosticsInfo resetDiagnosticsInfo();

    protected:

        std::tuple<bool, float> move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities);
        void resolve(const Hit& hit, Transform& tf, Rigidbody& rb, Rigidbody& otherRb, const glm::vec3& movement);
        void addGravity(Entity entity, Transform& tf, Rigidbody& rb, float dt);
        void updateCurrentUpdateInfo(const std::chrono::nanoseconds& updateTime);

        std::vector<Collision> m_detectedCollisions;
        DiagnosticsInfo m_diagnosticsInfo;
        UpdateInfo m_currentUpdateInfo;

    private:

        Text& ensureDebugText();

        glm::vec3 m_gravity;

        Actor m_debugTextActor;
    };
}


#endif //ENGINE_PHYSICSSYSTEMBASE_H
