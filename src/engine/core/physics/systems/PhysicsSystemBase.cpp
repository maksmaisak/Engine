//
// Created by Maksym Maisak on 2019-04-01.
//

#include "PhysicsSystemBase.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <chrono>
#include <locale>
#include "Transform.h"
#include "Rigidbody.h"
#include "Hit.h"
#include "Messaging.h"
#include "Collision.h"
#include "StringName.h"
#include "UIRect.h"
#include "Text.h"

using namespace en;

namespace {

    /// A helper for resolving collisions between physical bodies
    /// in a way that obeys conservation of momentum.
    /// Assumes `normal` is normalized.
    inline void updateVelocities(
        glm::vec3& aVelocity, float aInverseMass,
        glm::vec3& bVelocity, float bInverseMass,
        const glm::vec3& normal, float bounciness = 1.f
    ) {
        assert(glm::epsilonEqual(glm::length(normal), 1.f, 0.0001f));

        const float aSpeedAlongNormal = glm::dot(normal, aVelocity);
        const float bSpeedAlongNormal = glm::dot(normal, bVelocity);
        if (aSpeedAlongNormal - bSpeedAlongNormal > 0.f)
            return;

        const float u =
            (aSpeedAlongNormal * bInverseMass + bSpeedAlongNormal * aInverseMass) /
            (aInverseMass + bInverseMass);

        const float aDeltaSpeedAlongNormal = -(1.f + bounciness) * (aSpeedAlongNormal - u);
        aVelocity += normal * aDeltaSpeedAlongNormal;

        const float bDeltaSpeedAlongNormal = -(1.f + bounciness) * (bSpeedAlongNormal - u);
        bVelocity += normal * bDeltaSpeedAlongNormal;
    }
}

std::tuple<bool, float> PhysicsSystemBase::move(Entity entity, Transform& tf, Rigidbody& rb, float dt, EntitiesView<Transform, Rigidbody>& entities) {

    const glm::vec3 movement = rb.velocity * dt;

    if (rb.collider) {

        for (Entity other : entities) {

            if (entity == other)
                continue;

            auto& otherRb = m_registry->get<Rigidbody>(other);
            if (!otherRb.collider)
                continue;

            m_currentUpdateInfo.numCollisionChecks += 1;
            std::optional<Hit> optionalHit = rb.collider->collide(*otherRb.collider, movement);
            if (!optionalHit)
                continue;

            m_currentUpdateInfo.numCollisions += 1;
            const Hit& hit = *optionalHit;
            resolve(hit, tf, rb, otherRb, movement);
            m_detectedCollisions.emplace_back(hit, entity, other);
            return {true, dt * (1.f - hit.timeOfImpact)};
            // TODO BUG in the case of discrete collision detection, this ignores all other potential collisions.
            // Evident when a body is sliding across the floor. The collision with the floor is resolved correctly but it ignores other bodies.
        }
    }

    tf.move(movement);
    return {false, 0.f};
}


void PhysicsSystemBase::resolve(const Hit& hit, Transform& tf, Rigidbody& rb, Rigidbody& otherRb, const glm::vec3& movement) {

    const float otherInvMass = otherRb.isStatic ? 0.f : otherRb.invMass;
    updateVelocities(
        rb.velocity, rb.invMass,
        otherRb.velocity, otherInvMass,
        hit.normal, std::min(rb.bounciness, otherRb.bounciness)
    );
    if (otherRb.isStatic)
        otherRb.velocity = glm::vec3(0);

    tf.move(movement * hit.timeOfImpact + hit.depenetrationOffset);
    rb.collider->updateTransform(tf.getWorldTransform());
}

void PhysicsSystemBase::addGravity(Entity entity, Transform& tf, Rigidbody& rb, float dt) {

    rb.velocity += m_gravity * dt;

    /*for (Entity other : m_registry->with<Transform, Rigidbody>()) {

        if (entity == other) continue;

        auto& otherRb = m_registry->get<Rigidbody>(other);
        auto& otherTf = m_registry->get<Transform>(other);

        rb.velocity += dt * 1.f / (otherRb.invMass * glm::distance2(tf.getWorldPosition(), otherTf.getWorldPosition()));
    }*/
}

void PhysicsSystemBase::updateCurrentUpdateInfo(const std::chrono::nanoseconds& updateTime) {

    using namespace std::literals::string_literals;
    using namespace std::chrono;
    using ms = duration<double, std::milli>;

    auto& i = m_diagnosticsInfo;
    auto& u = m_currentUpdateInfo;

    u.time = updateTime;
    i.updateTimes.push_back(updateTime);
    i.updateTimeAverage.addSample(updateTime);
    i.updateTimeMin = std::min(m_diagnosticsInfo.updateTimeMin, updateTime);
    i.updateTimeMax = std::max(m_diagnosticsInfo.updateTimeMax, updateTime);

    std::stringstream s;
    s <<
      "Physics:\n" <<
      "update time          : " << duration_cast<ms>(u.time).count() << "ms" << std::endl <<
      "update time (average): " << duration_cast<ms>(i.updateTimeAverage.get()).count() << "ms" << std::endl <<
      "update time (max)    : " << duration_cast<ms>(i.updateTimeMax).count() << "ms" << std::endl <<
      "update time (min)    : " << duration_cast<ms>(i.updateTimeMin).count() << "ms" << std::endl <<
      "collision checks: " << u.numCollisionChecks << std::endl <<
      "collisions      : " << u.numCollisions << std::endl;
    //std::cout << s.str();
    ensureDebugText().setString(s.str());

    m_currentUpdateInfo = {};
}

Text& PhysicsSystemBase::ensureDebugText() {

    if (!m_debugTextActor.isValid()) {
        m_debugTextActor = m_engine->makeActor("PhysicsSystemDebug");
    }

    m_debugTextActor.getOrAdd<Transform>();
    auto& rect = m_debugTextActor.getOrAdd<UIRect>();
    rect.offsetMin = rect.offsetMax = {30, -30};

    if (auto* textPtr = m_debugTextActor.tryGet<Text>())
        return *textPtr;

    return m_debugTextActor.add<Text>()
        .setString("Test")
        .setAlignment({0, 1})
        .setFont(Resources<sf::Font>::get(config::FONT_PATH + "Menlo.ttc"));
}

const PhysicsSystemBase::DiagnosticsInfo& PhysicsSystemBase::getDiagnosticsInfo() const {
    return m_diagnosticsInfo;
}

PhysicsSystemBase::DiagnosticsInfo PhysicsSystemBase::resetDiagnosticsInfo() {

    auto copy = m_diagnosticsInfo;
    m_diagnosticsInfo = {};
    return copy;
}

PhysicsSystemBase& PhysicsSystemBase::setGravity(const glm::vec3& gravity) {

    m_gravity = gravity;
    return *this;
}