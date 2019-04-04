//
// Created by Maksym Maisak on 2019-04-04.
//

#ifndef ENGINE_OCTREENODE_H
#define ENGINE_OCTREENODE_H

#include <memory>
#include <vector>
#include <array>
#include "glm.hpp"
#include "Entity.h"
#include "Bounds.h"

namespace en {

    class OctreeNode {

    public:
        OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int maxDepth = 4, int depth = 0);
        std::size_t getTotalNumEntities() const;
        const std::array<std::unique_ptr<OctreeNode>, 8>& getChildren() const;
        utils::Bounds getChildBounds(int childIndex);

        void add   (Entity entity, const utils::Bounds& bounds);
        void remove(Entity entity, const utils::Bounds& searchInBounds);
        void update(Entity entity, const utils::Bounds& oldBounds, const utils::Bounds& newBounds);

    private:

        OctreeNode& ensureChildNode(int childIndex);
        std::unique_ptr<OctreeNode> makeChild(int childIndex);

        glm::vec3 m_center;
        glm::vec3 m_halfSize;
        int m_maxDepth = -1;
        int m_depth    = -1;

        std::array<std::unique_ptr<OctreeNode>, 8> m_children;
        std::vector<Entity> m_entities;
    };
}

#endif //ENGINE_OCTREENODE_H
