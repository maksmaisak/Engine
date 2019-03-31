//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef ENGINE_PHYSICSTESTSCENE_H
#define ENGINE_PHYSICSTESTSCENE_H

#include "PhysicsTestBodyGenerator.h"

class PhysicsTestScene : public en::Scene {

public:
    struct Preset {
        int numBodiesStatic  = 100;
        int numBodiesDynamic = 100;
        glm::vec3 fieldHalfSize = glm::vec3(50);
    };

    // Clang bug doesn't allow to use {} for the default member initializers
    PhysicsTestScene(const Preset& preset = {100, 100, glm::vec3(50)});
    void open() override;
    void update(float dt) override;

private:
    void addStaticBodies();
    void addDynamicBodies();

    Preset m_preset;
    en::PhysicsTestBodyGenerator m_bodyGenerator;
};


#endif //ENGINE_PHYSICSTESTSCENE_H
