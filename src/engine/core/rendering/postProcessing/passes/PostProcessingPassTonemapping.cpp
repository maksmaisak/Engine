//
// Created by Maksym Maisak on 3/11/19.
//

#include "PostProcessingPassTonemapping.h"
#include <imgui.h>
#include "TextureObject.h"
#include "ShaderProgram.h"
#include "PostProcessingUtilities.h"

using namespace en;

PostProcessingPassTonemapping::PostProcessingPassTonemapping() :
    m_exposure(1.f)
{}

void PostProcessingPassTonemapping::displayImGui() {

    if (ImGui::Begin("Post Processing")) {
        if (ImGui::CollapsingHeader("Tonemapping")) {
            if (updateIsEnabled()) {
                ImGui::SliderFloat("Exposure", &m_exposure, 0.02f, 5.f);
            }
        }
    }
    ImGui::End();
}

void PostProcessingPassTonemapping::render(const gl::TextureObject& sourceTexture) {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/tonemapping");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");
    static const GLint exposureLocation = shader->getUniformLocation("exposure");

    shader->use();
    gl::setUniform(sourceTextureLocation, sourceTexture, 0);
    gl::setUniform(exposureLocation, m_exposure);
    PostProcessingUtilities::renderQuad();
}