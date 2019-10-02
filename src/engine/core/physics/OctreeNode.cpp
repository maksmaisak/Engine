//
// Created by Maksym Maisak on 2019-04-04.
//

#include "OctreeNode.h"
#include <cassert>
#include <numeric>
#include "Exception.h"

using namespace en;

OctreeNode::OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int maxDepth, std::size_t capacity, int depth) :
    m_center(center),
    m_halfSize(halfSize),
    m_maxDepth(maxDepth),
    m_capacity(capacity),
    m_depth(depth)
{
    assert(depth <= maxDepth);
}

OctreeNode::OctreeNode(OctreeNode* parent, const glm::vec3& center, const glm::vec3& halfSize) :
    m_parent(parent),
    m_center(center),
    m_halfSize(halfSize),
    m_depth(parent->m_depth + 1),
    m_maxDepth(parent->m_maxDepth),
    m_capacity(parent->m_capacity)
{
    assert(m_depth <= m_maxDepth);
}

std::size_t OctreeNode::getTotalNumEntities() const {

    std::size_t count = m_entities.size();
    for (int i = 0; i < 8; ++i)
        if (m_children[i])
            count += m_children[i]->getTotalNumEntities();

    return count;
}

const std::array<std::unique_ptr<en::OctreeNode>, 8>& OctreeNode::getChildren() const {return m_children;}

const std::vector<std::pair<Entity, utils::Bounds>> OctreeNode::getEntities() const {return m_entities;}

std::size_t OctreeNode::getCapacity() const {return m_capacity;}

utils::Bounds OctreeNode::getBounds() const {

    return {
        m_center - m_halfSize,
        m_center + m_halfSize
    };
}

/// Use this instead of calling getBounds on a child to preserve cache.
utils::Bounds OctreeNode::getChildBounds(int childIndex) const {

    const glm::vec3 min = {
        (childIndex & 1) ? m_center.x : m_center.x - m_halfSize.x,
        (childIndex & 2) ? m_center.y : m_center.y - m_halfSize.y,
        (childIndex & 4) ? m_center.z : m_center.z - m_halfSize.z
    };

    const glm::vec3 max = {
        (childIndex & 1) ? m_center.x + m_halfSize.x : m_center.x,
        (childIndex & 2) ? m_center.y + m_halfSize.y : m_center.y,
        (childIndex & 4) ? m_center.z + m_halfSize.z : m_center.z
    };

    return {min, max};
}

void OctreeNode::add(Entity entity, const utils::Bounds& bounds) {

    if (!isLeafNode()) {

        // TODO instead of checking against each child's bounds, efficiently set a bitmask with 8 bits in one go.
        for (int i = 0; i < 8; ++i)
            if (getChildBounds(i).intersects(bounds))
                ensureChildNode(i).add(entity, bounds);

    } else {

        addAndSplitIfNeeded(entity, bounds);
    }
}

void OctreeNode::remove(Entity entity, const utils::Bounds& searchInBounds) {

    if (!isLeafNode()) {

        for (int i = 0; i < 8; ++i)
            if (m_children[i] && getChildBounds(i).intersects(searchInBounds))
                m_children[i]->remove(entity, searchInBounds);

    } else if (getBounds().intersects(searchInBounds)) {

        removeAndMergeParentIfNeeded(entity);
    }
}

void OctreeNode::update(Entity entity, const utils::Bounds& oldBounds, const utils::Bounds& newBounds) {

    // Remove from nodes that intersect the oldBounds BUT not the newBounds.
    // Add to nodes that intersect the newBounds BUT not the oldBounds.

    if (!isLeafNode()) {

        for (int i = 0; i < 8; ++i) {

            const utils::Bounds& childBounds = getChildBounds(i);

            const bool wasInChild      = m_children[i] && childBounds.intersects(oldBounds);
            const bool shouldBeInChild = childBounds.intersects(newBounds);

            if (!wasInChild && !shouldBeInChild)
                continue;

            if (m_children[i])
                m_children[i]->update(entity, oldBounds, newBounds);
            else
                ensureChildNode(i).add(entity, newBounds);
        };
        return;
    }

    const utils::Bounds& bounds = getBounds();
    const bool wasHere = oldBounds.intersects(bounds);
    const bool shouldBeHere = newBounds.intersects(bounds);

    if (!wasHere) {
        if (shouldBeHere) {
            add(entity, bounds);
        }
    } else {
        if (!shouldBeHere) {
            remove(entity, bounds);
        } else {
            auto it = std::find_if(m_entities.begin(), m_entities.end(), [entity](const auto& pair) {
                return pair.first == entity;
            });

            if (it != m_entities.end())
                it->second = newBounds;
        }
    }
}

OctreeNode& OctreeNode::ensureChildNode(int childIndex) {

    const std::unique_ptr<OctreeNode>& ptr = m_children[childIndex];
    if (!ptr) {
        ++m_numChildren;
        return *(m_children[childIndex] = makeChild(childIndex));
    }

    return *ptr;
}

std::unique_ptr<OctreeNode> OctreeNode::makeChild(int childIndex) {

    const glm::vec3 childHalfSize = m_halfSize * 0.5f;
    const glm::vec3 offset = {
        (childIndex & 1) ? childHalfSize.x : -childHalfSize.x,
        (childIndex & 2) ? childHalfSize.y : -childHalfSize.y,
        (childIndex & 4) ? childHalfSize.z : -childHalfSize.z
    };

    return std::make_unique<OctreeNode>(this, m_center + offset, childHalfSize);
}

bool OctreeNode::isLeafNode() const {
    return m_numChildren == 0;
}

void OctreeNode::removeIf(const std::function<bool(Entity, const utils::Bounds&)>& condition) {

    if (!isLeafNode()) {

        for (int i = 0; i < 8; ++i)
            if (const std::unique_ptr<OctreeNode>& childPtr = m_children[i])
                childPtr->removeIf(condition);
    }

    if (isLeafNode()) {

        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(), [&condition](const auto& pair) {
                return condition(pair.first, pair.second);
            }),
            m_entities.end()
        );

        if (m_parent)
            m_parent->mergeIfNeeded();
    }
}

void OctreeNode::addAndSplitIfNeeded(Entity entity, const utils::Bounds& bounds) {

    const auto it = std::find_if(m_entities.begin(), m_entities.end(), [entity](const auto& pair){
        return pair.first == entity;
    });
    if (it != m_entities.end())
        return;

    m_entities.emplace_back(entity, bounds);

    splitIfNeeded();
}

void OctreeNode::removeAndMergeParentIfNeeded(Entity entity) {

    assert(isLeafNode());

    const auto it = std::find_if(m_entities.begin(), m_entities.end(), [entity](const auto& pair){return pair.first == entity;});
    if (it == m_entities.end())
        return;

    m_entities.erase(it);

    if (m_parent)
        m_parent->mergeIfNeeded();
}

void OctreeNode::splitIfNeeded() {

    assert(isLeafNode());

    if (m_depth == m_maxDepth)
        return;

    if (m_entities.size() <= m_capacity)
        return;

    for (int i = 0; i < 8; ++i) {

        const utils::Bounds childBounds = getChildBounds(i);

        for (const auto& [e, entityBounds] : m_entities)
            if (childBounds.intersects(entityBounds))
                ensureChildNode(i).add(e, entityBounds);
    }
    m_entities.clear();
}

void OctreeNode::mergeIfNeeded() {

    // TODO Fix this includes duplicates
    std::size_t numEntities = getTotalNumEntities();
    if (numEntities > m_capacity)
        return;

    for (int i = 0; i < 8; ++i) {

        if (!m_children[i])
            continue;

        assert(m_children[i]->isLeafNode());

        const auto& childEntities = m_children[i]->m_entities;
        m_entities.reserve(m_entities.size() + childEntities.size());
        std::copy(childEntities.begin(), childEntities.end(), std::back_inserter(m_entities));

        m_children[i] = nullptr;
        --m_numChildren;
    }

    // Remove duplicates
    std::sort(m_entities.begin(), m_entities.end(), [](const auto& pairA, const auto& pairB){
        return pairA.first < pairB.first;
    });
    m_entities.erase(
        std::unique(m_entities.begin(), m_entities.end(), [](const auto& pairA, const auto& pairB){
            return pairA.first == pairB.first;
        }),
        m_entities.end()
    );
}

void OctreeNode::printWithIndent(std::ostream& stream, int indentDepth) const {

    std::string indent {};
    for (int i = 0; i < indentDepth * 4; ++i)
        indent.push_back(' ');

    stream << indent << "node entities: " << m_entities.size() << ", children:\n";
    for (int i = 0; i < 8; ++i)
        if (m_children[i])
            m_children[i]->printWithIndent(stream, indentDepth + 1);
}
