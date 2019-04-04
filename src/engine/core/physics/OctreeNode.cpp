//
// Created by Maksym Maisak on 2019-04-04.
//

#include "OctreeNode.h"

using namespace en;

std::size_t OctreeNode::getTotalNumEntities() const {

    std::size_t count = m_children.size();
    for (int i = 0; i < 8; ++i)
        if (m_children[i])
            count += m_children[i]->getTotalNumEntities();

    return count;
}

const std::array<std::unique_ptr<en::OctreeNode>, 8>& OctreeNode::getChildren() const {

    return m_children;
}
