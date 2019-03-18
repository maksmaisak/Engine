//
// Created by Maksym Maisak on 2019-03-13.
//

#include "SkyboxRenderer.h"
#include <cassert>
#include <array>
#include "Resources.h"
#include "GLSetUniform.h"
#include "GLHelpers.h"

using namespace en;

namespace {

    const std::array<float, 3 * 36> skyboxVertices {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
}

SkyboxRenderer::SkyboxRenderer() : m_shader(Resources<ShaderProgram>::get("skybox")) {

    glGenBuffers(1, &m_bufferId);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
        glCheckError();
        glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);
        glCheckError();
        glEnableVertexAttribArray(0);
        glCheckError();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glCheckError();
    }
    glBindVertexArray(0);

    glCheckError();
}

SkyboxRenderer::~SkyboxRenderer() {

    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_bufferId);
}

void SkyboxRenderer::draw(const Texture& cubemap, const glm::mat4& matrixPV) {

    assert(cubemap.isValid() && cubemap.getKind() == Texture::Kind::TextureCube);
    assert(m_shader);

    glDepthFunc(GL_LEQUAL);

    m_shader->use();
    glCheckError();
    glBindVertexArray(m_vao);
    {
        gl::setUniform(m_shader->getUniformLocation("matrixPV"), matrixPV);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getId());
        glUniform1i(m_shader->getUniformLocation("skyboxTexture"), 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glCheckError();
    }
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}
