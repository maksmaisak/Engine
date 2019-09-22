//
// Created by Maksym Maisak on 2019-09-22.
//

#include "RenderingSharedState.h"

using namespace en;

RenderingSharedState::RenderingSharedState() :
    depthMaps(4, {1024, 1024}, 10, {64, 64}),
    referenceResolution(1920, 1080)
{}
