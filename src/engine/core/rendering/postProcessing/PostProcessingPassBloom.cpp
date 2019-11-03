//
// Created by Maksym Maisak on 2/11/19.
//

#include "PostProcessingPassBloom.h"
#include <algorithm>
#include <array>
#include <imgui.h>
#include <cmath>
#include "PostProcessingUtilities.h"
#include "Engine.h"
#include "ScopedBind.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Resources.h"
#include "ShaderProgram.h"
#include "GLHelpers.h"

using namespace en;

namespace {

    constexpr std::size_t MaxKernelSize = 64;
    using Kernel = std::array<float, MaxKernelSize>;

    float gauss(std::size_t i, float standardDeviation = 1.f) {

        const float s = standardDeviation;
        const float x = i;

        // would prefer constexpr, but std::sqrt isn't constexpr
        static const float sqrt2Pi = std::sqrt(6.28318530717958647692528676655900576f);

        const float value = std::exp(x * x / (-2.f * s * s)) / (s * sqrt2Pi);
        return value;
    }

    void doPass(const std::shared_ptr<ShaderProgram>& shader, GLint uniformLocation, const gl::TextureObject& sourceTexture) {

        shader->use();
        gl::setUniform(uniformLocation, sourceTexture, 0);
        PostProcessingUtilities::renderQuad();
    }

    void blitFrom(const gl::TextureObject& sourceTexture) {

        static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("blit");
        static const GLint textureLocation = shader->getUniformLocation("sourceTexture");
        doPass(shader, textureLocation, sourceTexture);
    }

    void isolateBrightFragments(const gl::TextureObject& sourceTexture, const gl::FramebufferObject& target, float brightnessThreshold = 1.f) {

        static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/isolateBrightFragments");
        static const GLint textureLocation = shader->getUniformLocation("sourceTexture");
        static const GLint brightnessThresholdLocation = shader->getUniformLocation("threshold");

        shader->use();
        gl::setUniform(textureLocation, sourceTexture, 0);
        gl::setUniform(brightnessThresholdLocation, brightnessThreshold);

        const gl::ScopedBind bindFBO(target, GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        PostProcessingUtilities::renderQuad();
    }

    template<bool horizontal>
    void blurDimensionFrom(const gl::TextureObject& sourceTexture, GLint kernelSize, const Kernel* kernel) {

        static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader(horizontal ?
            "postProcessing/bloom/blurHorizontal" :
            "postProcessing/bloom/blurVertical"
        );
        static const GLint textureLocation = shader->getUniformLocation("sourceTexture");
        static const GLint kernelSizeLocation = shader->getUniformLocation("kernelSize");
        static const std::array<GLint, MaxKernelSize> kernelLocations = [&]() {
            std::array<GLint, MaxKernelSize> locations {};
            std::generate(locations.begin(), locations.end(), [&, i = 0]() mutable {
                return shader->getUniformLocation("kernel[" + std::to_string(i++) + "]");
            });
            return locations;
        }();
        static const GLint maxSupportedKernelSize = [&]() {
            const auto firstUnsupportedValueIt = std::find_if(kernelLocations.begin(), kernelLocations.end(), [](GLint l) { return l < 0; });
            return std::min<GLint>(MaxKernelSize, static_cast<GLint>(firstUnsupportedValueIt - kernelLocations.begin()));
        }();

        const GLint usedKernelSize = std::min(maxSupportedKernelSize, kernelSize);

        shader->use();
        gl::setUniform(textureLocation, sourceTexture, 0);
        gl::setUniform(kernelSizeLocation, usedKernelSize);

        if (kernel) {
            for (GLint i = 0; i < usedKernelSize; i++) {
                gl::setUniform(kernelLocations[i], (*kernel)[i]);
            }
        }

        PostProcessingUtilities::renderQuad();
    }
}

void PostProcessingPassBloom::setUp(const glm::u32vec2& size) {

    for (gl::FramebufferBundle& fb : m_blurFramebuffers) {
        fb = PostProcessingUtilities::makeFramebuffer(size);
    }
}

void PostProcessingPassBloom::render(const gl::TextureObject& sourceTexture) {

    updateSettings();

    isolateBrightFragments(sourceTexture, m_blurFramebuffers[1].framebuffer, m_settings.brightnessThreshold);
    blur();
    bloomCombine(sourceTexture, m_blurFramebuffers[1].colorTexture);
}

void PostProcessingPassBloom::updateSettings() {

    if (ImGui::Begin("Post Processing")) {

        if (ImGui::BeginChild("Bloom")) {

            ImGui::Text("Filtering");
            ImGui::SliderFloat("Brightness threshold", &m_settings.brightnessThreshold, 0.f, 10.f);

            ImGui::Text("Blur");
            ImGui::SliderFloat("Standard deviation", &m_settings.blurStandardDeviation, 0.01f, 10.f);
            ImGui::SliderInt("Kernel size", &m_settings.blurKernelSize, 1, MaxKernelSize);
            ImGui::SliderInt("Num iterations", &m_settings.numBlurIterations, 1, 10);

            ImGui::Text("Contribution");
            ImGui::SliderFloat("Bloom intensity", &m_settings.intensity, 0.f, 5.f);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void PostProcessingPassBloom::blur() {

    static const std::shared_ptr<ShaderProgram> shader = PostProcessingUtilities::getPostProcessingShader("postProcessing/bloom/gaussianBlur");
    static const GLint sourceTextureLocation = shader->getUniformLocation("sourceTexture");
    static const GLint isHorizontalLocation = shader->getUniformLocation("isHorizontal");
    static const GLint kernelSizeLocation = shader->getUniformLocation("kernelSize");
    static const std::array<GLint, MaxKernelSize> kernelLocations = []() {
        std::array<GLint, MaxKernelSize> locations {};
        std::generate(locations.begin(), locations.end(), [i = 0]() mutable {
            return shader->getUniformLocation("kernel[" + std::to_string(i++) + "]");
        });
        return locations;
    }();
    static const GLint maxSupportedKernelSize = [&]() {
        const auto firstUnsupportedValueIt = std::find_if(kernelLocations.begin(), kernelLocations.end(), [](GLint l) { return l < 0; });
        return std::min<GLint>(MaxKernelSize, static_cast<GLint>(firstUnsupportedValueIt - kernelLocations.begin()));
    }();

    shader->use();
    const GLint usedKernelSize = std::min(maxSupportedKernelSize, m_settings.blurKernelSize);
    gl::setUniform(kernelSizeLocation, usedKernelSize);
    for (GLint i = 0; i < usedKernelSize; i++) {
        gl::setUniform(kernelLocations[i], gauss(i, m_settings.blurStandardDeviation));
    }

    const auto blurDimension = [&](bool isHorizontal) {

        gl::setUniform(isHorizontalLocation, isHorizontal ? 1 : 0);
        gl::setUniform(sourceTextureLocation, m_blurFramebuffers[isHorizontal ? 1 : 0].colorTexture, 0);

        const gl::ScopedBind bindFBO(m_blurFramebuffers[isHorizontal ? 0 : 1].framebuffer, GL_FRAMEBUFFER);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        PostProcessingUtilities::renderQuad();
    };

    for (int i = 0; i < m_settings.numBlurIterations; ++i) {

        blurDimension(true);
        blurDimension(false);
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

    glEnable(GL_FRAMEBUFFER_SRGB);
    PostProcessingUtilities::renderQuad();
    glDisable(GL_FRAMEBUFFER_SRGB);
}
