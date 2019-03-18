//
// Created by Maksym Maisak on 2019-03-06.
//

#ifndef SAXION_Y2Q2_RENDERING_MODEL_H
#define SAXION_Y2Q2_RENDERING_MODEL_H

#include <memory>
#include <vector>
#include <string>
#include "Mesh.hpp"

namespace en {

    /// A collection of meshes imported from a file.
    class Model {

        friend struct ModelImpl;

    public:
        static std::shared_ptr<Model> load(const std::string& filepath);
        Model() = default;

        const std::vector<Mesh>& getMeshes() const;

    private:
        std::string m_filepath;
        std::vector<Mesh> m_meshes;
    };
}

#endif //SAXION_Y2Q2_RENDERING_MODEL_H
