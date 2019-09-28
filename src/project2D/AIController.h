//
// Created by Maksym Maisak on 2019-09-28.
//

#ifndef ENGINE_AICONTROLLER_H
#define ENGINE_AICONTROLLER_H

#include "Behavior.h"

namespace en {
    class Engine;
}

class AIController : public en::Behavior {

    using Behavior::Behavior;
public:
    static AIController& create(class en::Engine& engine);

    void update(float dt) override;

private:

};


#endif //ENGINE_AICONTROLLER_H
