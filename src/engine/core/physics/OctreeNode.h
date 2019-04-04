//
// Created by Maksym Maisak on 2019-04-04.
//

#ifndef ENGINE_OCTREENODE_H
#define ENGINE_OCTREENODE_H

#include <memory>
#include <vector>
#include <array>
#include "Entity.h"

namespace en {

    class OctreeNode {

    public:
        std::size_t getTotalNumEntities() const;
        const std::array<std::unique_ptr<OctreeNode>, 8>& getChildren() const;

    private:
        std::array<std::unique_ptr<OctreeNode>, 8> m_children;
        std::vector<Entity> m_entities;
    };
}

#endif //ENGINE_OCTREENODE_H
