//
// Created by Maksym Maisak on 2/10/19.
//

#include "FramebufferObject.h"

GLuint gl::makeFBO() {

    GLuint id;
    glGenFramebuffers(1, &id);
    return id;
}

void gl::deleteFBO(GLuint id) {

    glDeleteFramebuffers(1, &id);
}

void gl::FramebufferObject::bind(GLenum target) const {

    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    glCheckError();
}

void gl::FramebufferObject::unbind(GLenum target) const {

    glCheckError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCheckError();
}