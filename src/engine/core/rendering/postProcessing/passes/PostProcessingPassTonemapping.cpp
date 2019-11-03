//
// Created by Maksym Maisak on 3/11/19.
//

#include "PostProcessingPassTonemapping.h"
#include <imgui.h>
#include "TextureObject.h"
#include "ShaderProgram.h"
#include "PostProcessingUtilities.h"

using namespace en;

void PostProcessingPassTonemapping::displayImGui() {

    if (ImGui::Begin("Post Processing")) {
        if (ImGui::CollapsingHeader("Tonemapping")) {
            updateIsEnabled();
        }
    }
    ImGui::End();
}

void PostProcessingPassTonemapping::render(const gl::TextureObject& sourceTexture) {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/tonemapping");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");

    shader->use();
    gl::setUniform(sourceTextureLocation, sourceTexture, 0);
    PostProcessingUtilities::renderQuad();
}
