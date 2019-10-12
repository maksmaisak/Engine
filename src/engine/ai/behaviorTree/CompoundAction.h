//
// Created by Maksym Maisak on 9/10/19.
//

#ifndef ENGINE_COMPOUNDACTION_H
#define ENGINE_COMPOUNDACTION_H

#include <vector>
#include <memory>
#include <type_traits>
#include "Action.h"

namespace ai {

    class CompoundAction : public Action {

    public:
        CompoundAction() = default;

        template<typename... TActions>
        CompoundAction(std::unique_ptr<TActions>... actions) {

            std::unique_ptr<Action> baseActions[] {std::move(actions)...};
            for (std::unique_ptr<Action>& action : baseActions) {
                m_actions.push_back(std::move(action));
            }
        }

        void reset() override;

        CompoundAction& addAction(std::unique_ptr<Action> action);

    protected:
        std::vector<std::unique_ptr<Action>> m_actions;
    };
}

#endif //ENGINE_COMPOUNDACTION_H
