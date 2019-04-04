//
// Created by Maksym Maisak on 2019-04-04.
//

#include "OctreeNode.h"
#include <cassert>
#include "Exception.h"

using namespace en;

std::size_t OctreeNode::getTotalNumEntities() const {

    std::size_t count = m_entities.size();
    for (int i = 0; i < 8; ++i)
        if (m_children[i])
            count += m_children[i]->getTotalNumEntities();

    return count;
}

const std::array<std::unique_ptr<en::OctreeNode>, 8>& OctreeNode::getChildren() const {

    return m_children;
}

OctreeNode::OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int maxDepth, int depth) :
    m_center(center),
    m_halfSize(halfSize),
    m_maxDepth(maxDepth),
    m_depth(depth)
{
    assert(depth <= maxDepth);
}

utils::Bounds OctreeNode::getChildBounds(int childIndex) {

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

    if (m_depth == m_maxDepth) {
        m_entities.push_back(entity);
    }

    int intersectsChildIndex = -1;
    bool intersectsMoreThanOneChild = false;
    for (int i = 0; i < 8; ++i) {

        if (bounds.intersect(getChildBounds(i))) {

            if (intersectsChildIndex == -1) {
                intersectsChildIndex = i;
            } else {
                intersectsMoreThanOneChild = true;
                break;
            }
        }
    }

    if (intersectsMoreThanOneChild || intersectsChildIndex == -1) {
        m_entities.push_back(entity);
    } else {
        ensureChildNode(intersectsChildIndex).add(entity, bounds);
    }
}

void OctreeNode::remove(Entity entity, const utils::Bounds& searchInBounds) {

    throw utils::Exception("Not implemented");
    // TODO remove from nodes that intersect the bounds.
}

void OctreeNode::update(Entity entity, const utils::Bounds& oldBounds, const utils::Bounds& newBounds) {

    throw utils::Exception("Not implemented");
    // TODO remove from nodes that intersect the oldBounds BUT not the newBounds.
    // Add to nodes that intersect the newBounds BUT not the oldBounds.
}

OctreeNode& OctreeNode::ensureChildNode(int childIndex) {

    const std::unique_ptr<OctreeNode>& ptr = m_children[childIndex];
    if (!ptr)
        return *(m_children[childIndex] = makeChild(childIndex));

    return *ptr;
}

std::unique_ptr<OctreeNode> OctreeNode::makeChild(int childIndex) {

    const glm::vec3 childHalfSize = m_halfSize * 0.5f;
    const glm::vec3 offset = {
        (childIndex & 1) ? childHalfSize.x : -childHalfSize.x,
        (childIndex & 2) ? childHalfSize.y : -childHalfSize.y,
        (childIndex & 4) ? childHalfSize.z : -childHalfSize.z
    };

    return std::make_unique<OctreeNode>(m_center + offset, childHalfSize, m_maxDepth, m_depth + 1);
}
