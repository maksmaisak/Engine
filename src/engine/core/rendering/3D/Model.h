//
// Created by Maksym Maisak on 2019-03-06.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include <memory>
#include <vector>
#include <string>
#include "Name.h"
#include "Mesh.h"
#include "Resources.h"

namespace en {

    /// A collection of meshes imported from a file.
    class Model {

        friend struct ModelImpl;

    public:
        static std::shared_ptr<Model> load(const Name& filepath);
        Model() = default;

        const std::vector<Mesh>& getMeshes() const;

    private:
        Name m_filepath;
        std::vector<Mesh> m_meshes;
    };

    using Models = Resources<Model>;
}

#endif //ENGINE_MODEL_H
