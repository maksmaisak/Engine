//
// Created by Maksym Maisak on 2019-09-22.
//

#ifndef ENGINE_RENDERINGSHAREDSTATE_H
#define ENGINE_RENDERINGSHAREDSTATE_H

#include "DepthMaps.h"
#include "VertexRenderer.h"

namespace en {

    /// The state shared between all rendering systems
    struct RenderingSharedState {

        RenderingSharedState();

        DepthMaps depthMaps;
        VertexRenderer vertexRenderer;

        glm::vec2 referenceResolution;
    };
}

#endif //ENGINE_RENDERINGSHAREDSTATE_H
