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

void deleteVAO(GLuint id) {

    glDeleteVertexArrays(1, &id);
}

void VertexBufferObject::bind(GLenum target) {assert(m_id); glBindBuffer(target, m_id);}
void VertexBufferObject::unbind(GLenum target) {glBindBuffer(target, 0);}

void TextureObject::bind(GLenum target) {assert(m_id); glBindTexture(target, m_id);}
void TextureObject::unbind(GLenum target) {glBindTexture(target, 0);}

void VertexArrayObject::bind() {assert(m_id); glBindVertexArray(m_id);}
void VertexArrayObject::unbind() {glBindVertexArray(0);}
