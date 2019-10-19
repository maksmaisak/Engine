//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_SHOOTACTION_H
#define ENGINE_SHOOTACTION_H

#include "Action.h"
#include "glm.h"
#include "Name.h"

namespace ai {

    class ShootAction : public Action {

    public:
        explicit ShootAction(const en::Name& targetPositionName);
        ActionOutcome execute() override;

    private:
        en::Name m_targetPositionName;
    };
}

#endif //ENGINE_SHOOTACTION_H
