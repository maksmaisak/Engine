//
// Created by Maksym Maisak on 2019-03-06.
//

#include "Model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include "Mesh.hpp"

using namespace en;

namespace en {

    struct ModelImpl {

        using index_t = unsigned int;

        inline static void addNode(Model& model, const aiScene* scene, const aiNode* node, const aiMatrix4x4& parentTransform = {}) {

            const aiMatrix4x4 transform = parentTransform * node->mTransformation;

            for (index_t i = 0; i < node->mNumMeshes; ++i) {
                const index_t index  = node ->mMeshes[i];
                const aiMesh* aiMesh = scene->mMeshes[index];
                model.m_meshes.emplace_back(aiMesh, transform);
            }

            for (index_t i = 0; i < node->mNumChildren; ++i) {
                addNode(model, scene, node->mChildren[i], transform);
            }
        }
    };
}

#ifdef AI_CONFIG_PP_RVC_FLAGS
    #undef AI_CONFIG_PP_RVC_FLAGS
#endif
#define AI_CONFIG_PP_RVC_FLAGS aiComponent_MATERIALS | aiComponent_COLORS | aiComponent_ANIMATIONS | aiComponent_BONEWEIGHTS | aiComponent_LIGHTS | aiComponent_TEXTURES | aiComponent_CAMERAS

std::shared_ptr<Model> Model::load(const std::string& filepath) {

    auto model = std::make_shared<Model>();
    model->m_filepath = filepath;

    const auto postProcessSteps =
        aiProcess_Triangulate |
        aiProcess_RemoveComponent |
        aiProcess_OptimizeMeshes |
        aiProcess_OptimizeGraph  |
        aiProcess_CalcTangentSpace;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, postProcessSteps);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {

        std::cerr << "Could not load model from " << filepath << ":" << std::endl;
        std::cerr << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    ModelImpl::addNode(*model, scene, scene->mRootNode);

    std::cout << "Loaded model with " << model->m_meshes.size() << " mesh(es) from " << filepath << std::endl;

    return model;
}

const std::vector<Mesh>& Model::getMeshes() const {
    return m_meshes;
}
