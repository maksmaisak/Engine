//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingPassBloom.h"
#include <algorithm>
#include <imgui.h>
#include <cmath>
#include "PostProcessingUtilities.h"
#include "ScopedBind.h"
#include "ShaderProgram.h"
#include "GLHelpers.h"

using namespace en;

namespace {

    constexpr std::size_t MaxKernelSize = 64;

    /// The gaussian distribution (aka normal distribution)
    float gauss(std::size_t i, float standardDeviation = 1.f) {

        const float s = standardDeviation;
        const float x = i;

        // would prefer constexpr, but std::sqrt isn't constexpr
        static const float sqrt2Pi = std::sqrt(6.28318530717958647692528676655900576f);

        const float value = std::exp(x * x / (-2.f * s * s)) / (s * sqrt2Pi);
        return value;
    }

    std::array<GLint, MaxKernelSize> getKernelLocations(const ShaderProgram& shader) {

        std::array<GLint, MaxKernelSize> locations {};
        std::generate(locations.begin(), locations.end(), [&, i = 0]() mutable {
            return shader.getUniformLocation("kernel[" + std::to_string(i++) + "]");
        });
        return locations;
    }

    GLint getMaxUsableKernelSize(const std::array<GLint, MaxKernelSize>& kernelLocations) {

        const auto firstUnsupportedValueIt = std::find_if(
            kernelLocations.begin(),
            kernelLocations.end(),
            [](GLint l) { return l < 0; }
        );

        const GLint maxSupportedKernelSize = static_cast<GLint>(firstUnsupportedValueIt - kernelLocations.begin());
        return std::min(static_cast<GLint>(MaxKernelSize), maxSupportedKernelSize);
    }
}

void PostProcessingPassBloom::setUp(const glm::u32vec2& size) {

    for (gl::FramebufferBundle& fb : m_blurFramebuffers) {
        fb = PostProcessingUtilities::makeFramebuffer(size);
    }
}

void PostProcessingPassBloom::render(const gl::TextureObject& sourceTexture) {

    isolateBrightFragments(sourceTexture, m_blurFramebuffers[1].framebuffer);
    blur();
    bloomCombine(sourceTexture, m_blurFramebuffers[1].colorTexture);
}

void PostProcessingPassBloom::displayImGui() {

    if (ImGui::Begin("Post Processing")) {

        if (ImGui::CollapsingHeader("Bloom")) {

            if (updateIsEnabled()) {

                ImGui::Text("Filtering");
                ImGui::SliderFloat("Brightness threshold", &m_settings.brightnessThreshold, 0.f, 10.f);

                ImGui::Text("Blur");
                ImGui::SliderFloat("Standard deviation", &m_settings.blurStandardDeviation, 0.01f, 10.f);
                ImGui::SliderInt("Kernel size", &m_settings.blurKernelSize, 1, MaxKernelSize);
                ImGui::SliderInt("Num iterations", &m_settings.numBlurIterations, 1, 10);

                ImGui::Text("Contribution");
                ImGui::SliderFloat("Bloom intensity", &m_settings.intensity, 0.f, 5.f);
            }
        }
    }
    ImGui::End();
}

void PostProcessingPassBloom::isolateBrightFragments(const gl::TextureObject& sourceTexture, const gl::FramebufferObject& target) {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/isolateBrightFragments");
    static const GLint textureLocation = shader->getUniformLocation("sourceTexture");
    static const GLint brightnessThresholdLocation = shader->getUniformLocation("threshold");

    shader->use();
    gl::setUniform(textureLocation, sourceTexture, 0);
    gl::setUniform(brightnessThresholdLocation, m_settings.brightnessThreshold);

    const gl::ScopedBind bindFBO(target, GL_FRAMEBUFFER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    PostProcessingUtilities::renderQuad();
}

void PostProcessingPassBloom::blur() {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/gaussianBlur");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");
    static const GLint isHorizontalLocation = shader->getUniformLocation("isHorizontal");
    static const GLint kernelSizeLocation = shader->getUniformLocation("kernelSize");
    static const std::array<GLint, MaxKernelSize> kernelLocations = getKernelLocations(*shader);
    static const GLint maxUsableKernelSize = getMaxUsableKernelSize(kernelLocations);

    shader->use();
    const GLint usedKernelSize = std::min(maxUsableKernelSize, m_settings.blurKernelSize);
    gl::setUniform(kernelSizeLocation, usedKernelSize);
    for (GLint i = 0; i < usedKernelSize; i++) {
        gl::setUniform(kernelLocations[i], gauss(i, m_settings.blurStandardDeviation));
    }

    const auto doBlurPass = [&](bool isHorizontal, const gl::TextureObject& source, const gl::FramebufferObject& target) {

        gl::setUniform(isHorizontalLocation, isHorizontal ? 1 : 0);
        gl::setUniform(sourceTextureLocation, source, 0);

        const gl::ScopedBind bindFBO(target, GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        PostProcessingUtilities::renderQuad();
    };

    for (int i = 0; i < m_settings.numBlurIterations; ++i) {
        doBlurPass(true , m_blurFramebuffers[1].colorTexture, m_blurFramebuffers[0].framebuffer);
        doBlurPass(false, m_blurFramebuffers[0].colorTexture, m_blurFramebuffers[1].framebuffer);
    }
}

void PostProcessingPassBloom::bloomCombine(const gl::TextureObject& originalTexture, const gl::TextureObject& blurredTexture) {

    static const std::shared_ptr<ShaderProgram> bloomCombine = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/bloomCombine");
    static const GLint sourceTextureLocation  = bloomCombine->getUniformLocation("sourceTexture");
    static const GLint blurredTextureLocation = bloomCombine->getUniformLocation("blurredTexture");
    static const GLint intensityLocation = bloomCombine->getUniformLocation("intensity");

    bloomCombine->use();
    gl::setUniform(intensityLocation, m_settings.intensity);
    gl::setUniform(sourceTextureLocation, originalTexture, 0);
    gl::setUniform(blurredTextureLocation, blurredTexture, 1);

    PostProcessingUtilities::renderQuad();
}
