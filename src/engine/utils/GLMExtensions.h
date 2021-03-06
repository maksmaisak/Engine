//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_GLMEXTENSIONS_H
#define ENGINE_GLMEXTENSIONS_H

#include "glm.h"

namespace glm {

    template<length_t L, typename T, qualifier Q>
    glm::vec<L, T, Q> truncate(const glm::vec<L, T, Q>& vector, T maxLength) {

        const T length = glm::length(vector);
        if (length < glm::epsilon<T>()) {
            return glm::vec<L, T, Q>(static_cast<T>(0));
        }

        if (length < maxLength) {
            return vector;
        }

        return vector * maxLength / length;
    }

    template<length_t L, typename T, qualifier Q>
    glm::vec<L, T, Q> moveTowards(const glm::vec<L, T, Q>& current, const glm::vec<L, T, Q>& target, T maxDistance) {

        const glm::vec<L, T, Q> delta = target - current;
        const T length = glm::length(delta);
        if (length < glm::epsilon<T>()) {
            return target;
        }

        if (length > maxDistance) {
            return current + delta * maxDistance / length;
        }

        return current + delta;
    }
}

#endif //ENGINE_GLMEXTENSIONS_H
