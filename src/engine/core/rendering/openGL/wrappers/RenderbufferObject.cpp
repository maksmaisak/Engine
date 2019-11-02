//
// Created by Maksym Maisak on 2/11/19.
//

#include "RenderbufferObject.h"

GLuint gl::makeRBO() {

    GLuint id;
    glGenRenderbuffers(1, &id);
    return id;
}

void gl::deleteRBO(GLuint id) {

    glDeleteRenderbuffers(1, &id);
}

void gl::RenderbufferObject::bind(GLenum target) const {

    glCheckError();
    glBindRenderbuffer(target, m_id);
    glCheckError();
}

void gl::RenderbufferObject::unbind(GLenum target) const {

    glCheckError();
    glBindRenderbuffer(target, 0);
    glCheckError();
}