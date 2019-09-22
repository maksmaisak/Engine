//
// Created by Maksym Maisak on 2019-03-11.
//

#ifndef ENGINE_RENDERSETTINGS_H
#define ENGINE_RENDERSETTINGS_H

#include <memory>
#include "config.hpp"
#include "glm.h"
#include "Texture.h"

namespace en {

    struct RenderSettings {

        glm::vec3 ambientColor = {0, 0, 0};
        std::shared_ptr<Texture> skyboxTexture;
        bool useSkybox = true;
    };
}

#endif //ENGINE_RENDERSETTINGS_H
