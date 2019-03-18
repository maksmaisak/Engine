//
// Created by Maksym Maisak on 2019-01-09.
//

#ifndef SAXION_Y2Q2_RENDERING_GLSETUNIFORM_H
#define SAXION_Y2Q2_RENDERING_GLSETUNIFORM_H

#include "glm.hpp"
#include <GL/glew.h>

/// Template wrapper around some of the glUniform* functions.

namespace gl {

    // Default case, no implementation
    template<typename T>
    void setUniform(GLint location, const T& value);

    template<>
    inline void setUniform<GLint>(GLint location, const GLint& value) {
        glUniform1i(location, value);
    }

    template<>
    inline void setUniform<GLuint>(GLint location, const GLuint& value) {
        glUniform1ui(location, value);
    }

    template<>
    inline void setUniform<GLfloat>(GLint location, const GLfloat& value) {
        glUniform1f(location, value);
    }

    template<>
    inline void setUniform<glm::vec2>(GLint location, const glm::vec2& value) {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    template<>
    inline void setUniform<glm::vec3>(GLint location, const glm::vec3& value) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    template<>
    inline void setUniform<glm::vec4>(GLint location, const glm::vec4& value) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    template<>
    inline void setUniform<glm::mat4>(GLint location, const glm::mat4& value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    template<>
    inline void setUniform<glm::mat3>(GLint location, const glm::mat3& value) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

#endif //SAXION_Y2Q2_RENDERING_GLSETUNIFORM_H
