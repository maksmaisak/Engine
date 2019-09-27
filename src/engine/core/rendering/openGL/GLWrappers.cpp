//
// Created by Maksym Maisak on 2019-09-24.
//

#include "GLWrappers.h"

using namespace gl;

GLuint gl::createVBO() {

    GLuint id;
    glGenBuffers(1, &id);
    return id;
}

void gl::deleteVBO(GLuint id) {

    glDeleteBuffers(1, &id);
}


GLuint gl::createTexture() {

    GLuint id;
    glGenTextures(1, &id);
    return id;
}

void gl::deleteTexture(GLuint id) {

    glDeleteTextures(1, &id);
}


GLuint gl::createVAO() {

    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

void gl::deleteVAO(GLuint id) {

    glDeleteVertexArrays(1, &id);
}


void VertexBufferObject::bind(GLenum target) const {assert(isValid()); glBindBuffer(target, m_id);}
void VertexBufferObject::unbind(GLenum target) const {glBindBuffer(target, 0);}

void TextureObject::bind(GLenum target) const {assert(isValid()); glBindTexture(target, m_id);}
void TextureObject::unbind(GLenum target) const {glBindTexture(target, 0);}

void VertexArrayObject::bind() const {assert(isValid()); glBindVertexArray(m_id);}
void VertexArrayObject::unbind() const {glBindVertexArray(0);}
