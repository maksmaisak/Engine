//
// Created by Maksym Maisak on 22/12/18.
//

#ifndef ENGINE_TRANSFORMABLE_H
#define ENGINE_TRANSFORMABLE_H

#include <vector>
#include "glm.h"
#include <glm/gtc/quaternion.hpp>

#include "Entity.h"
#include "ComponentsToLua.h"
#include "LuaStack.h"

namespace en {

    class Transform final {

        LUA_TYPE(Transform);

        friend class TransformHierarchySystem;

    public:

        static void initializeMetatable(class LuaState& lua);
        static Transform& addFromLua(class Actor& actor, class LuaState& lua);

        explicit Transform(const glm::mat4& localTransform);
        explicit Transform(const glm::vec3& position = glm::vec3(0.f), const glm::quat& rotation = glm::quat(1.f, 0.f, 0.f, 0.f), const glm::vec3& scale = glm::vec3(1.f));

        Transform(const Transform& other) = delete;
        Transform& operator=(const Transform& other) = delete;
        Transform(Transform&& other) = default;
        Transform& operator=(Transform&& other) = default;
        ~Transform() = default;

        const glm::mat4& getLocalTransform() const;
        const glm::mat4& getWorldTransform() const;

        inline const glm::vec3& getLocalPosition() const {return m_position;}
        inline const glm::quat& getLocalRotation() const {return m_rotation;}
        inline const glm::vec3& getLocalScale   () const {return m_scale;}

        inline Entity getParent() const {return m_parent;}
        inline const std::vector<Entity>& getChildren() const {return m_children;}

        glm::vec3 getWorldPosition() const;
        glm::quat getWorldRotation() const;
        glm::vec3 getForward() const;

        // TODO add setWorldPosition/Rotation
        Transform& setLocalPosition(const glm::vec3& localPosition);
        Transform& setLocalPosition2D(const glm::vec2& localPosition);
        Transform& setLocalRotation(const glm::quat& localRotation);
        Transform& setLocalScale(const glm::vec3& localScale);
        Transform& setLocalScale(float localScale);

        Transform& setParent(Entity newParent);

        Transform& move(const glm::vec3& offset);
        Transform& move(const glm::vec2& offset);
        Transform& move(float x, float y, float z = 0.0f);
        Transform& rotate(const glm::quat& offset);
        Transform& rotate(float angle, const glm::vec3& axis);
        Transform& scale(const glm::vec3& scaleMultiplier);
        Transform& scale(float scaleMultiplier);

    private:

        void addChild(Entity child);
        void removeChild(Entity child);

        class EntityRegistry* m_registry = nullptr;
        class Engine* m_engine = nullptr;
        Entity m_entity = en::nullEntity;
        Entity m_parent = en::nullEntity;
        std::vector<Entity> m_children;

        glm::vec3 m_position = {0, 0, 0};
        glm::quat m_rotation = {1, 0, 0, 0};
        glm::vec3 m_scale    = {1, 1, 1};

        mutable glm::mat4 m_matrixLocal = glm::mat4(1.f);
        mutable bool m_matrixLocalDirty = true;

        mutable glm::mat4 m_matrixWorld = glm::mat4(1.f);
        mutable bool m_matrixWorldDirty = true;

        void markDirty();
        void markWorldDirty();
    };
}

#endif //ENGINE_TRANSFORMABLE_H
