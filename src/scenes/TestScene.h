//
// Created by Maksym Maisak on 27/12/18.
//

#ifndef SAXION_Y2Q2_RENDERING_TESTSCENE_H
#define SAXION_Y2Q2_RENDERING_TESTSCENE_H

#include <vector>
#include <memory>
#include "Model.h"
#include "RenderInfo.h"
#include "Rigidbody.h"
#include "Engine.h"
#include "Scene.h"

class TestScene : public en::Scene {

public:
    TestScene();
    void open() override;
    void update(float dt) override;

private:

    void setUpNonBodies();

    void makeSphere(const glm::vec3& position);
    void makeCube  (const glm::vec3& position);

    en::RenderInfo m_sphereRenderInfo;
    en::RenderInfo m_cubeRenderInfo;
    en::RenderInfo m_floorRenderInfo;
};


#endif //SAXION_Y2Q2_RENDERING_TESTSCENE_H
