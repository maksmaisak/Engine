//
// Created by Maksym Maisak on 2019-04-04.
//

#include "Bounds.h"

using namespace utils;

bool Bounds::intersect(const Bounds& other) const {

    for (int i = 0; i < 3; ++i)
        if (other.min[i] > max[i] || other.max[i] < min[i])
            return false;

    return true;
}
