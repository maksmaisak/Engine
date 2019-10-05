//
// Created by Maksym Maisak on 5/10/19.
//

#include "VertexArrayObject.h"

GLuint gl::createVAO() {

    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

void gl::deleteVAO(GLuint id) {

    glDeleteVertexArrays(1, &id);
}

void gl::VertexArrayObject::bind() const {

    glCheckError();
    assert(isValid());
    glBindVertexArray(m_id);
    glCheckError();
}

void gl::VertexArrayObject::unbind() const {

    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}
