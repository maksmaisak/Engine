//
// Created by Maksym Maisak on 2/10/19.
//

#include "FramebufferObject.h"

using namespace gl;

void FramebufferObject::bind(GLenum target) const {

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FramebufferObject::unbind(GLenum target) const {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
