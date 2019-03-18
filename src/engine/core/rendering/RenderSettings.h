//
// Created by Maksym Maisak on 2019-03-11.
//

#ifndef SAXION_Y2Q2_RENDERING_RENDERSETTINGS_H
#define SAXION_Y2Q2_RENDERING_RENDERSETTINGS_H

#include <memory>
#include "config.hpp"
#include "glm.hpp"
#include "Texture.hpp"

namespace en {

    struct RenderSettings {

        glm::vec3 ambientColor = {0, 0, 0};
        std::shared_ptr<Texture> skyboxTexture;
        bool useSkybox = true;
    };
}

#endif //SAXION_Y2Q2_RENDERING_RENDERSETTINGS_H
