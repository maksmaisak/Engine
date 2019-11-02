//
// Created by Maksym Maisak on 2019-09-22.
//

#ifndef ENGINE_RENDERINGSHAREDSTATE_H
#define ENGINE_RENDERINGSHAREDSTATE_H

#include <unordered_map>
#include "DepthMaps.h"
#include "Mesh.h"
#include "VertexRenderer.h"
#include "FramebufferBundle.h"

namespace en {

    /// The state shared between all rendering systems
    struct RenderingSharedState {

        RenderingSharedState();
        void loadConfigFromLua(class LuaState& lua);

        DepthMaps depthMaps;
        VertexRenderer vertexRenderer;
        std::unordered_map<std::shared_ptr<class Material>, Mesh> batches;

        /// If this is set to a valid framebuffer, everything will be rendered into it instead of the screen.
        /// In that case the framebuffer should be used as input for post-processing.
        gl::FramebufferBundle prePostProcessingFramebuffer;

        bool enableStaticBatching;
        bool enableDebugOutput;
        glm::vec2 referenceResolution;
    };
}

#endif //ENGINE_RENDERINGSHAREDSTATE_H
