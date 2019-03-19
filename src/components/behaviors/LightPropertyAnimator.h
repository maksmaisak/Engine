//
// Created by Maksym Maisak on 2019-01-13.
//

#ifndef SAXION_Y2Q2_RENDERING_LIGHTPROPERTYANIMATOR_H
#define SAXION_Y2Q2_RENDERING_LIGHTPROPERTYANIMATOR_H

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
    sf::Time m_startTime;
};

#endif //SAXION_Y2Q2_RENDERING_LIGHTPROPERTYANIMATOR_H
