//
// Created by Maksym Maisak on 9/10/19.
//

#include "CompoundAction.h"

using namespace ai;

void CompoundAction::reset() {

    Action::reset();

    for (std::unique_ptr<Action>& action : m_actions) {
        if (action) {
            action->reset();
        }
    }
}

CompoundAction& CompoundAction::addAction(std::unique_ptr<Action> action) {

    m_actions.push_back(std::move(action));
    return *this;
}
