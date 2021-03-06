//
// Created by Maksym Maisak on 2019-04-04.
//

#ifndef ENGINE_OCTREENODE_H
#define ENGINE_OCTREENODE_H

#include <memory>
#include <vector>
#include <functional>
#include <array>
#include <ostream>
#include "glm.h"
#include "Entity.h"
#include "Bounds.h"

namespace en {

    class OctreeNode {

    public:
        OctreeNode(const glm::vec3& center, const glm::vec3& halfSize, int maxDepth = 3, std::size_t capacity = 10, int depth = 0);
        OctreeNode(OctreeNode* parent, const glm::vec3& center, const glm::vec3& halfSize);
        const std::array<std::unique_ptr<OctreeNode>, 8>& getChildren() const;
        const std::vector<std::pair<Entity, utils::Bounds>> getEntities() const;
        std::size_t getTotalNumEntities() const;
        std::size_t getCapacity() const;
        utils::Bounds getBounds() const;
        utils::Bounds getChildBounds(int childIndex) const;
        bool isLeafNode() const;

        void add   (Entity entity, const utils::Bounds& bounds);
        void remove(Entity entity, const utils::Bounds& searchInBounds);
        void update(Entity entity, const utils::Bounds& oldBounds, const utils::Bounds& newBounds);

        void removeIf(const std::function<bool(Entity, const utils::Bounds& bounds)>& condition);

    private:

        OctreeNode& ensureChildNode(int childIndex);
        std::unique_ptr<OctreeNode> makeChild(int childIndex);

        void mergeIfNeeded();
        void splitIfNeeded();
        void addAndSplitIfNeeded(Entity entity, const utils::Bounds& bounds);
        void removeAndMergeParentIfNeeded(Entity entity);

        inline friend std::ostream& operator<<(std::ostream& stream, const OctreeNode& node) {
            return node.printWithIndent(stream, 0), stream;
        }
        void printWithIndent(std::ostream& stream, int indent) const;

        glm::vec3 m_center;
        glm::vec3 m_halfSize;
        int m_maxDepth    = -1;
        int m_depth       = -1;
        std::size_t m_capacity = 10;

        int m_numChildren = 0;
        std::array<std::unique_ptr<OctreeNode>, 8> m_children;
        OctreeNode* m_parent = nullptr;

        std::vector<std::pair<Entity, utils::Bounds>> m_entities;
    };
}

#endif //ENGINE_OCTREENODE_H
