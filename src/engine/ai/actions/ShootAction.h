//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_SHOOTACTION_H
#define ENGINE_SHOOTACTION_H

#include "Action.h"
#include "glm.h"

namespace ai {

    class ShootAction : public Action {

    public:
        ShootAction(const glm::vec2& targetPosition);
        ActionOutcome execute() override;

    private:
        glm::vec2 m_targetPosition;
    };
}

#endif //ENGINE_SHOOTACTION_H
