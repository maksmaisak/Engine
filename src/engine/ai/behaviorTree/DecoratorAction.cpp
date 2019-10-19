//
// Created by Maksym Maisak on 12/10/19.
//

#include "DecoratorAction.h"

using namespace ai;

DecoratorAction::DecoratorAction(std::unique_ptr<Action> child) :
    m_child(std::move(child))
{}

void DecoratorAction::reset() {

    Action::reset();

    if (m_child) {
        m_child->reset();
    }
}
