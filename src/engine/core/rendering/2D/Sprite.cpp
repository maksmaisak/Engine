//
// Created by Maksym Maisak on 17/10/19.
//

#include "Sprite.h"

using namespace en;

utils::Bounds2D Sprite::getAABB(const glm::mat4& matrixModel) const {

    utils::Bounds2D bounds {
        glm::vec2(std::numeric_limits<float>::max()),
        glm::vec2(std::numeric_limits<float>::lowest())
    };

    // Add the corners in worldspace
    for (std::uint32_t i = 0b00; i <= 0b11; ++i) {
        bounds.add(matrixModel * glm::vec4(
            ((i & 0b10) ? 1.f : 0.f) - pivot.x,
            ((i & 0b01) ? 1.f : 0.f) - pivot.y,
            0.f,
            1.f
        ));
    }

    return bounds;
}

