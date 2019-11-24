//
// Created by Maksym Maisak on 2019-01-13.
//

#ifndef ENGINE_LIGHTPROPERTYANIMATOR_H
#define ENGINE_LIGHTPROPERTYANIMATOR_H

#include "Light.h"
#include "Behavior.h"
#include "GameTime.h"

class LightPropertyAnimator : public en::Behavior {

    using Behavior::Behavior;

public:
    virtual ~LightPropertyAnimator() = default;
    void start() override;
    void update(float dt) override;

private:
    en::Light m_initialLight;
    en::TimePoint m_startTime;
};

#endif //ENGINE_LIGHTPROPERTYANIMATOR_H
