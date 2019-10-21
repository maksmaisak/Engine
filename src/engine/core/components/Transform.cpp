//
// Created by Maksym Maisak on 22/12/18.
//

#include "Transform.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_query.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <optional>
#include <variant>
#include "ComponentsToLua.h"
#include "LuaStack.h"
#include "Tween.h"

using namespace en;

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
    m_position(position),
    m_rotation(rotation),
    m_scale(scale)
{}

Transform::Transform(const glm::mat4& localTransform) {

    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(localTransform, m_scale, m_rotation, m_position, skew, perspective);
}

const glm::mat4& Transform::getLocalTransform() const {

    if (m_matrixLocalDirty) {
        m_matrixLocal = glm::translate(m_position) * glm::toMat4(m_rotation) * glm::scale(m_scale);
        m_matrixLocalDirty = false;
    }

    return m_matrixLocal;
}

const glm::mat4& Transform::getWorldTransform() const {

    if (m_matrixWorldDirty) {

        if (!en::isNullEntity(m_parent)) {
            m_matrixWorld = m_registry->get<Transform>(m_parent).getWorldTransform() * getLocalTransform();
        } else {
            m_matrixWorld = getLocalTransform();
        }

        m_matrixWorldDirty = false;
    }

    return m_matrixWorld;
}

Transform& Transform::setLocalPosition(const glm::vec3& localPosition) {

    m_position = localPosition;
    m_matrixLocal[3] = {m_position, 1.f};
    markWorldDirty();
    return *this;
}

Transform& Transform::setLocalPosition2D(const glm::vec2& localPosition) {

    return setLocalPosition(glm::vec3(localPosition, m_position.z));
}

Transform& Transform::setLocalRotation(const glm::quat& localRotation) {

    m_rotation = localRotation;
    markDirty();
    return *this;
}

Transform& Transform::setLocalScale(const glm::vec3& localScale) {

    m_scale = localScale;
    markDirty();
    return *this;
}

Transform& Transform::setLocalScale(float localScale) {

    return setLocalScale(glm::vec3(localScale));
}

// TODO Make this optionally preserve world position
Transform& Transform::setParent(Entity newParent) {

    const Entity oldParent = m_parent;
    if (!isNullEntity(oldParent)) {
        if (oldParent == newParent) {
            return *this;
        }
        m_registry->get<Transform>(oldParent).removeChild(m_entity);
    }

    if (!isNullEntity(newParent)) {
        auto& parentTransform = m_registry->get<Transform>(newParent);
        parentTransform.addChild(m_entity);
    }

    m_parent = newParent;
    m_matrixWorldDirty = true;
    return *this;
}

void Transform::markDirty() {

    m_matrixLocalDirty = true;
    markWorldDirty();
}

void Transform::markWorldDirty() {

    m_matrixWorldDirty = true;

    // TODO remove children with no transform from the list of children
    for (Entity child : m_children)
        if (auto* childTf = m_registry->tryGet<Transform>(child))
            childTf->markWorldDirty();
}

void Transform::addChild(Entity child) {

    assert(std::find(m_children.cbegin(), m_children.cend(), child) == m_children.cend());
    m_children.push_back(child);
}

void Transform::removeChild(Entity child) {

    auto it = std::remove(m_children.begin(), m_children.end(), child);
    assert(it != m_children.end());
    m_children.erase(it, m_children.end());
}

Transform& Transform::move(const glm::vec3& offset) {

    m_position += offset;
    m_matrixLocal[3] = {m_position, 1.f};
    markWorldDirty();
    return *this;
}

Transform& Transform::move(const glm::vec2& offset) {

    return move(glm::vec3(offset, 0.f));
}

Transform& Transform::move(float x, float y, float z) {

    return move({x, y, z});
}

Transform& Transform::rotate(const glm::quat& offset) {

    m_rotation = offset * m_rotation;
    markDirty();
    return *this;
}

Transform& Transform::rotate(float angle, const glm::vec3& axis) {

    rotate(glm::angleAxis(angle, glm::normalize(axis)));
    return *this;
}

Transform& Transform::scale(const glm::vec3& scaleMultiplier) {

    m_scale *= scaleMultiplier;
    markDirty();
    return *this;
}

Transform& Transform::scale(float scaleMultiplier) {

    return scale(glm::vec3(scaleMultiplier));
}

glm::vec3 Transform::getWorldPosition() const {

    return glm::vec3(getWorldTransform()[3]);
}

glm::quat Transform::getWorldRotation() const {

    glm::vec3 scale;
    glm::quat orientation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(getWorldTransform(), scale, orientation, translation, skew, perspective);

    return orientation;
}

glm::vec3 Transform::getForward() const {

    // Since -z is forward, the forward direction of a matrix is the opposite of it's z axis
    // (the vector onto which it maps (0,0,1))
    auto dir = glm::vec3(getWorldTransform()[2]);
    dir.z = -dir.z;
    return glm::normalize(dir);
}

en::GridPosition Transform::getGridPosition() const {

    return glm::floor(getWorldPosition());
}

void Transform::initializeMetatable(LuaState& lua) {

    lua.setField("move", [](ComponentReference<Transform> tf, float x, float y, float z) {
        tf->move({x, y, z});
        return tf;
    });

    lua.setField("rotate", [](ComponentReference<Transform> tf, float angle, float x, float y, float z) {
        tf->rotate(glm::radians(angle), {x, y, z});
        return tf;
    });

    lua::addProperty(lua, "position", lua::property(
        [](ComponentReference<Transform> tf) {return tf->getLocalPosition();},
        [](ComponentReference<Transform> tf, const glm::vec3& pos) {tf->setLocalPosition(pos);}
    ));

    lua::addProperty(lua, "rotation", lua::property(
        [](ComponentReference<Transform> tf) {return glm::degrees(glm::eulerAngles(tf->getLocalRotation()));},
        [](ComponentReference<Transform> tf, const glm::vec3& eulerAngles) {tf->setLocalRotation(glm::radians(eulerAngles));}
    ));

    lua::addProperty(lua, "scale", lua::property(
        [](ComponentReference<Transform> tf) {return tf->getLocalScale();},
        [](ComponentReference<Transform> tf, const glm::vec3& scale) {tf->setLocalScale(scale);}
    ));

    lua::addProperty(lua, "parent", lua::property(
        [](ComponentReference<Transform> ref) -> std::optional<Actor> {

            Transform& tf = *ref;
            if (isNullEntity(tf.m_parent))
                return std::nullopt;

            return std::make_optional(ref->m_engine->actor(ref->m_parent));
        },
        [](ComponentReference<Transform> ref, std::optional<std::variant<std::string, Actor>> arg) {

            Transform& tf = *ref;

            if (!arg) {
                tf.setParent(nullEntity);
                return;
            }

            if (auto* parentName = std::get_if<std::string>(&*arg)) {
                Entity parent = ref->m_engine->findByName(*parentName);
                tf.setParent(parent);
            } else if (auto* actor = std::get_if<Actor>(&*arg)) {
                tf.setParent(*actor);
            }
        }
    ));

    lua.setField("tweenPosition", [](
        const ComponentReference<Transform>& ref,
        const glm::vec3& target,
        std::optional<float> duration,
        std::optional<ease::Ease> ease
    ){
        Transform& tf = *ref;
        Entity entity = ref.getEntity();
        const glm::vec3& start = tf.getLocalPosition();

        return Tween::make(*ref.getRegistry(), ref.getEntity(), duration, ease,
            [ref, start, delta = target - start](float t){
                ref->setLocalPosition(start + delta * t);
            }
        );
    });

    lua.setField("tweenScale", [](
        const ComponentReference<Transform>& ref,
        const glm::vec3& target,
        std::optional<float> duration,
        std::optional<ease::Ease> ease
    ){
        Transform& tf = *ref;
        const glm::vec3& start = tf.getLocalScale();

        return Tween::make(*ref.getRegistry(), ref.getEntity(), duration, ease,
            [ref, start, delta = target - start](float t){
                ref->setLocalScale(start + delta * t);
            }
        );
    });

    lua.setField("tweenRotation", [](
        const ComponentReference<Transform>& ref,
        const glm::vec3& targetEuler,
        std::optional<float> duration,
        std::optional<ease::Ease> ease
    ){
        Transform& tf = *ref;
        const glm::quat& start = tf.getLocalRotation();

        return Tween::make(*ref.getRegistry(), ref.getEntity(), duration, ease,
            [ref, start, target = glm::quat(glm::radians(targetEuler))](float t){
                ref->setLocalRotation(glm::slerp(start, target, t));
            }
        );
    });

    lua.setField("tweenJump", [](
        const ComponentReference<Transform>& ref,
        const glm::vec3& target,
        std::optional<float> jumpHeight,
        std::optional<float> duration
    ){
        Transform& tf = *ref;
        Entity entity = ref.getEntity();
        const glm::vec3& start = tf.getLocalPosition();

        static const auto easeHeight = [](float t){

            const float halfT = t * 2.f;
            return halfT < 1.f ? ease::outQuad(halfT) : ease::outQuad(2.f - halfT);
        };

        return Tween::make(*ref.getRegistry(), ref.getEntity(), duration, ease::inOutQuad,
            [ref, start, delta = target - start, jumpHeight = jumpHeight.value_or(1.f)](float t) {

                glm::vec3 pos = start + delta * t;
                pos.y = start.y + jumpHeight * easeHeight(t);
                ref->setLocalPosition(pos);
            }
        );
    });
}

namespace {

    void addChildrenFromLua(Actor& actor, Transform& transform, LuaState& lua) {

        if (!lua_istable(lua, -1) && !lua_isuserdata(lua, -1))
            return;

        lua_getfield(lua, -1, "children");
        auto pop = PopperOnDestruct(lua);

        if (lua_isnil(lua, -1))
            return;

        std::vector<Actor> children = ComponentsToLua::makeEntities(lua, actor.getEngine(), -1);
        for (Actor& child : children) {
            auto* childTransform = child.tryGet<Transform>();
            if (childTransform) {
                childTransform->setParent(actor);
            }
        }
    }
}

Transform& Transform::addFromLua(Actor& actor, LuaState& lua) {

    auto& transform = actor.add<Transform>();
    addChildrenFromLua(actor, transform, lua);
    return transform;
}
