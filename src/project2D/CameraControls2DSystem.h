//
// Created by Maksym Maisak on 2019-09-27.
//

#ifndef ENGINE_CAMERACONTROLS2DSYSTEM_H
#define ENGINE_CAMERACONTROLS2DSYSTEM_H

#include "System.h"
#include "Receiver.h"

class CameraControls2DSystem : public en::System {

public:
    void update(float dt) override;
};


#endif //ENGINE_CAMERACONTROLS2DSYSTEM_H
