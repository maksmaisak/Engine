//
// Created by Maksym Maisak on 3/11/19.
//

#include "PostProcessingPassTonemapping.h"
#include <imgui.h>
#include "TextureObject.h"
#include "ShaderProgram.h"
#include "PostProcessingUtilities.h"

using namespace en;

void PostProcessingPassTonemapping::render(const gl::TextureObject& sourceTexture) {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/tonemapping");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");

    static bool shouldUse = true;
    if (ImGui::Begin("Post Processing")) {
        if (ImGui::CollapsingHeader("Tonemapping")) {
            ImGui::Checkbox("Enabled", &shouldUse);
        }
    }
    ImGui::End();

    if (!shouldUse) {
        PostProcessingUtilities::blit(sourceTexture);
        return;
    }

    shader->use();
    gl::setUniform(sourceTextureLocation, sourceTexture, 0);
    PostProcessingUtilities::renderQuad();
}
