//
// Created by Maksym Maisak on 9/10/19.
//

#include "BehaviorTree.h"

using namespace ai;

BehaviorTree::BehaviorTree(std::unique_ptr<Action> root) :
    m_root(std::move(root))
{}

void BehaviorTree::execute(en::Actor& actor) {

    if (!m_root) {
        return;
    }

    const ActionOutcome outcome = m_root->execute(actor);
    if (outcome != ActionOutcome::InProgress) {
        m_root->reset();
    }
}