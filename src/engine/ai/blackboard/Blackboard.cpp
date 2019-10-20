//
// Created by Maksym Maisak on 12/10/19.
//

#include "Blackboard.h"

using namespace ai;

en::Actor Blackboard::getActorChecked(const en::Name& key) const {

    if (const std::optional<en::Actor> optional = get<en::Actor>(key)) {
        if (optional->isValid()) {
            return *optional;
        }
    }

    return {};
}
