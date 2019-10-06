//
// Created by Maksym Maisak on 2/10/19.
//

#ifndef ENGINE_FRAMEBUFFEROBJECT_H
#define ENGINE_FRAMEBUFFEROBJECT_H

#include "GLWrapper.h"

namespace gl {

    GLuint makeFBO();
    void deleteFBO(GLuint id);

    class FramebufferObject : public Wrapper<makeFBO, deleteFBO> {
    public:
        using Wrapper::Wrapper;
        void bind(GLenum target) const;
        void unbind(GLenum target) const;
    };
}

#endif //ENGINE_FRAMEBUFFEROBJECT_H
