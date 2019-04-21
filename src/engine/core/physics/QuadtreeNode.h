//
// Created by Maksym Maisak on 2019-04-21.
//

#ifndef ENGINE_QUADTREENODE_H
#define ENGINE_QUADTREENODE_H

#include <memory>
#include <vector>
#include <functional>
#include <array>
#include <ostream>
#include "glm.hpp"
#include "Entity.h"
#include "Bounds.h"

namespace en {

    class QuadtreeNode {

    public:
        QuadtreeNode(const glm::vec2& center, const glm::vec2& halfSize, int maxDepth = 3, std::size_t capacity = 10, int depth = 0);
        QuadtreeNode(QuadtreeNode* parent, const glm::vec2& center, const glm::vec2& halfSize);
        const std::array<std::unique_ptr<QuadtreeNode>, 4>& getChildren() const;
        const std::vector<std::pair<Entity, utils::Bounds2D>> getEntities() const;
        std::size_t getTotalNumEntities() const;
        std::size_t getCapacity() const;
        utils::Bounds2D getBounds() const;
        utils::Bounds2D getChildBounds(int childIndex) const;
        bool isLeafNode() const;

        void add   (Entity entity, const utils::Bounds2D& bounds);
        void remove(Entity entity, const utils::Bounds2D& searchInBounds);
        void update(Entity entity, const utils::Bounds2D& oldBounds, const utils::Bounds2D& newBounds);

        void removeIf(const std::function<bool(Entity, const utils::Bounds2D& bounds)>& condition);

    private:

        QuadtreeNode& ensureChildNode(int childIndex);
        std::unique_ptr<QuadtreeNode> makeChild(int childIndex);

        void mergeIfNeeded();
        void splitIfNeeded();
        void addAndSplitIfNeeded(Entity entity, const utils::Bounds2D& bounds);
        void removeAndMergeParentIfNeeded(Entity entity);

        inline friend std::ostream& operator<<(std::ostream& stream, const QuadtreeNode& node) {
            return node.printWithIndent(stream, 0), stream;
        }
        void printWithIndent(std::ostream& stream, int indent) const;

        glm::vec2 m_center;
        glm::vec2 m_halfSize;
        int m_maxDepth = -1;
        int m_depth    = -1;
        std::size_t m_capacity = 10;

        int m_numChildren = 0;
        std::array<std::unique_ptr<QuadtreeNode>, 4> m_children;
        QuadtreeNode* m_parent = nullptr;

        std::vector<std::pair<Entity, utils::Bounds2D>> m_entities;
    };
}

#endif //ENGINE_QUADTREENODE_H
