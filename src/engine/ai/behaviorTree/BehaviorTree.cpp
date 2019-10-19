//
// Created by Maksym Maisak on 9/10/19.
//

#include "BehaviorTree.h"

using namespace ai;

BehaviorTree::BehaviorTree(std::unique_ptr<Action> root, std::unique_ptr<Blackboard> blackboard) :
    m_root(std::move(root)),
    m_blackboard(std::move(blackboard))
{
    if (!m_blackboard) {
        m_blackboard = std::make_unique<Blackboard>();
    }
}

void BehaviorTree::execute(en::Actor& actor) {

    if (!m_root) {
        return;
    }

    const ActionOutcome outcome = m_root->execute(actor, *m_blackboard);
    if (outcome != ActionOutcome::InProgress) {
        m_root->reset();
    }
}

Blackboard& BehaviorTree::getBlackboard() {

    assert(m_blackboard);
    return *m_blackboard;
}

const Blackboard& BehaviorTree::getBlackboard() const {

    assert(m_blackboard);
    return *m_blackboard;
}
