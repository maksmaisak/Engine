//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <memory>
#include "Texture.h"
#include "Bounds.h"

namespace en {

    struct Sprite {

        Bounds2D getAABB(const glm::mat4& matrixModel) const;
        glm::mat4 getRenderMatrix(const glm::mat4& matrixModel) const;

        std::shared_ptr<Texture> texture = Textures::white();
        glm::vec4 color = glm::vec4(1.f);
        glm::vec2 pivot = glm::vec2(0.5f);
    };
}

#endif //ENGINE_SPRITE_H
