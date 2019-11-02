//
// Created by Maksym Maisak on 2019-09-28.
//

#include "GLSetUniform.h"
#include <cassert>

namespace gl {

    template<>
    void setUniform<GLint>(GLint location, const GLint& value) {
        glUniform1i(location, value);
    }

    template<>
    void setUniform<GLuint>(GLint location, const GLuint& value) {
        glUniform1ui(location, value);
    }

    template<>
    void setUniform<GLfloat>(GLint location, const GLfloat& value) {
        glUniform1f(location, value);
    }

    template<>
    void setUniform<glm::vec2>(GLint location, const glm::vec2& value) {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void setUniform<glm::vec3>(GLint location, const glm::vec3& value) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void setUniform<glm::vec4>(GLint location, const glm::vec4& value) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void setUniform<glm::mat4>(GLint location, const glm::mat4& value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    template<>
    void setUniform<glm::mat3>(GLint location, const glm::mat3& value) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void setUniform(GLint location, const TextureObject& texture, GLenum textureNum, GLenum target) {

        glCheckError();
        assert(texture);

        glActiveTexture(GL_TEXTURE0 + textureNum);
        glBindTexture(target, texture.getId());
        glUniform1i(location, 0);

        glCheckError();
    }

    void setUniform(GLint location, const en::Texture* texture, GLenum textureNum, GLenum target) {

        assert(texture && texture->isValid());
        return setUniform(location, texture->getGLTextureObject(), textureNum, target);
    }
}
