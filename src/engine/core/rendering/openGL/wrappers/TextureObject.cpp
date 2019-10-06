//
// Created by Maksym Maisak on 5/10/19.
//

#include "TextureObject.h"

GLuint gl::createTexture() {

    GLuint id;
    glGenTextures(1, &id);
    return id;
}

void gl::deleteTexture(GLuint id) {

    glDeleteTextures(1, &id);
}

void gl::TextureObject::bind(GLenum target) const {

    glCheckError();

    assert(isValid());
    glBindTexture(target, m_id);

    glCheckError();
}

void gl::TextureObject::unbind(GLenum target) const {

    glCheckError();

    glBindTexture(target, 0);

    glCheckError();
}