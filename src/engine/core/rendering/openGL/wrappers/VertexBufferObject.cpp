//
// Created by Maksym Maisak on 5/10/19.
//

#include "VertexBufferObject.h"

GLuint gl::createVBO() {

    GLuint id;
    glGenBuffers(1, &id);
    return id;
}

void gl::deleteVBO(GLuint id) {

    glDeleteBuffers(1, &id);
}

void gl::VertexBufferObject::bind(GLenum target) const {

    glCheckError();

    assert(isValid());
    glBindBuffer(target, m_id);

    glCheckError();
}

void gl::VertexBufferObject::unbind(GLenum target) const {

    glCheckError();

    glBindBuffer(target, 0);

    glCheckError();
}